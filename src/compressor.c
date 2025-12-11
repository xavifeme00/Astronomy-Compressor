#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "utils/header.h"
#include "utils/reader.h"
#include "utils/sample_predictor.h"
#include "utils/arithmetic_coder.h"

#define MAX_BITPLANE_SIZE 10485760  // 10 MB 

typedef struct {
    int bit;
    uint64_t ***residualsMatrix;
    uint16_t cm;
    Header header;
    char *buffer;
    size_t buffer_size;
    size_t bytes_used;
} BitplaneJob;


void *encode_bitplane_thread(void *arg) {
    BitplaneJob *job = (BitplaneJob*)arg;

    uint64_t c1 = 0, c2 = 0, c3 = 0;
    ArithmeticCoder coder;
    init_coder(&coder, job->cm, 4096, (uint8_t*)job->buffer, job->buffer_size);

    for (int z = 0; z < job->header.z; z++)
        for (int y = 0; y < job->header.y; y++)
            for (int x = 0; x < job->header.x; x++) {
                uint16_t symbol = (job->residualsMatrix[z][y][x] >> job->bit) & 1;
                get_context(job->cm, job->residualsMatrix, z, y, x, job->header.x - 1, &c1, &c2, &c3, job->bit);
                encode_symbol(&coder, symbol, c1, c2, c3, job->bit, y, x);
                update_freq(&coder, symbol, c1, c2, c3);
            }

    job->bytes_used = finalize_encoding(&coder);
    return NULL;
}

void *decode_bitplane_thread(void *arg) {
    BitplaneJob *job = (BitplaneJob*)arg;

    uint64_t c1 = 0, c2 = 0, c3 = 0;
    ArithmeticCoder coder;
    init_decoder(&coder, job->cm, 4096, (uint8_t*)job->buffer, job->buffer_size);

    for (int z = 0; z < job->header.z; z++)
        for (int y = 0; y < job->header.y; y++)
            for (int x = 0; x < job->header.x; x++) {
                get_context(job->cm, job->residualsMatrix, z, y, x, job->header.x - 1, &c1, &c2, &c3, job->bit);
                uint16_t bit_val = decode_symbol(&coder, c1, c2, c3, job->bit, y, x);
                update_freq(&coder, bit_val, c1, c2, c3);

                job->residualsMatrix[z][y][x] |= ((uint64_t)bit_val << job->bit);
            }

    finalize_decoding(&coder);
    return NULL;
}

int parse_int_weghts(const char *file, uint64_t *out, int expected) {
    FILE *f = fopen(file, "r");
    if (!f) {
        perror("Error opening weights file");
        return -1;
    }

    char buffer[4096];
    if (!fgets(buffer, sizeof(buffer), f)) {
        fclose(f);
        fprintf(stderr, "Error reading weights file\n");
        return -1;
    }
    fclose(f);

    char *copy = strdup(buffer);
    if (!copy) return -1;

    char *token = strtok(copy, ",");
    int count = 0;

    while (token != NULL) {
        if (count >= expected) { 
            free(copy); 
            return -1; 
        }
        out[count++] = (uint64_t)atoi(token);
        token = strtok(NULL, ",");
    }

    int sum = 0;
    for (int i = 0; i < expected; i++) {
        sum += out[i];
    }
    if (sum != 1000) {
        fprintf(stderr, "Error: sum of weights must be 1000 (got %d)\n", sum);
        free(copy);
        return -1;
    }

    free(copy);
    return (count == expected) ? 0 : -1;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s cmp/dec <input_file> <output_file> <cm> <weights_file>\n", argv[0]);
        return 1;
    }

    char *type = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];
    uint16_t cm = (uint16_t)atoi(argv[4]);

    uint64_t weights[40];
    char *weights_file = argv[5];
    if (parse_int_weghts(weights_file, weights, 40) != 0) {
        fprintf(stderr, "Error: expected 32 comma-separated integers\n");
        return 1;
    }

    if (strcmp(type, "cmp") == 0) {
        Header header;
        uint64_t ***matrix = NULL;

        if (header_parse_filename(input_file, &header) != 0) {
            fprintf(stderr, "Error parsing header\n");
            return 1;
        }

        if (get_image_from_file(header, input_file, &matrix) != 0) {
            fprintf(stderr, "Error reading image data\n");
            return 1;
        }

        uint64_t ***residualsMatrix = NULL;
        if (initialize_image_matrix(header.z, header.y, header.x, &residualsMatrix) != 0) {
            fprintf(stderr, "Error initializing residuals matrix\n");
            return 1;
        }

        uint16_t min = 0;
        uint16_t max = (1 << header.bits) - 1;
        for (int z = 0; z < header.z; z++)
            for (int y = 0; y < header.y; y++)
                for (int x = 0; x < header.x; x++) {
                    uint64_t current = matrix[z][y][x];
                    uint64_t prediction = predict(matrix, header.z, header.y, header.x, z, y, x, weights);
                    int32_t diff = (int32_t)(current - prediction);
                    residualsMatrix[z][y][x] = map_difference(diff, prediction, min, max);
                }

        FILE *out = fopen(output_file, "wb");
        if (!out) { perror("Error opening output file"); return 1; }
        fwrite(&header, sizeof(Header), 1, out);

        // --- Multithreading ---
        pthread_t threads[header.bits];
        BitplaneJob jobs[header.bits];

        for (int bit = header.bits - 1; bit >= 0; bit--) {
            jobs[bit].bit = bit;
            jobs[bit].residualsMatrix = residualsMatrix;
            jobs[bit].header = header;
            jobs[bit].cm = cm;
            jobs[bit].buffer = malloc(MAX_BITPLANE_SIZE);
            jobs[bit].buffer_size = MAX_BITPLANE_SIZE;
            if (!jobs[bit].buffer) { fprintf(stderr, "Memory allocation error\n"); return 1; }

            pthread_create(&threads[bit], NULL, encode_bitplane_thread, &jobs[bit]);
        }

        for (int bit = header.bits - 1; bit >= 0; bit--) {
            pthread_join(threads[bit], NULL);
        }

        for (int bit = header.bits - 1; bit >= 0; bit--)
            fwrite(&jobs[bit].bytes_used, sizeof(size_t), 1, out);

        for (int bit = header.bits - 1; bit >= 0; bit--) {
            fwrite(jobs[bit].buffer, 1, jobs[bit].bytes_used, out);
            free(jobs[bit].buffer);
        }

        fclose(out);

        for (int z = 0; z < header.z; z++) {
            for (int y = 0; y < header.y; y++) {
                free(matrix[z][y]);
                free(residualsMatrix[z][y]);
            }
            free(matrix[z]);
            free(residualsMatrix[z]);
        }
        free(matrix);
        free(residualsMatrix);

    } else { // Decoder
        FILE *in = fopen(input_file, "rb");
        if (!in) { perror("open"); return 1; }

        Header header;
        size_t *bp_sizes;
        if (header_parse_compressed(in, &header, &bp_sizes) != 0) return 1;

        uint64_t ***matrix = NULL;
        uint64_t ***residualsMatrix = NULL;
        if (initialize_image_matrix(header.z, header.y, header.x, &matrix) != 0 ||
            initialize_image_matrix(header.z, header.y, header.x, &residualsMatrix) != 0) {
            fprintf(stderr, "Error initializing matrices\n");
            return 1;
        }

        char *bp_buffer[header.bits];
        for (int bit = header.bits - 1; bit >= 0; bit--) {
            bp_buffer[bit] = malloc(bp_sizes[bit]);
            if (!bp_buffer[bit]) { fprintf(stderr, "Memory allocation error\n"); return 1; }
            if (fread(bp_buffer[bit], 1, bp_sizes[bit], in) != bp_sizes[bit]) {
                fprintf(stderr, "Error reading bitplane %d\n", bit);
                return 1;
            }
        }

        fclose(in);

        uint64_t ****residualsMatrixThreads = malloc(header.bits * sizeof(uint64_t***));

        for (int t = 0; t < header.bits; t++) {
            if (initialize_image_matrix(header.z, header.y, header.x, &residualsMatrixThreads[t]) != 0) {
                fprintf(stderr, "Error initializing residualsMatrix for thread %d\n", t);
                return 1;
            }
        }

        pthread_t threads[header.bits];
        BitplaneJob jobs[header.bits];

        // Decodificación bitplane a bitplane, sin threads
        for (int bit = 0; bit < header.bits; bit++) {
            jobs[bit].bit = bit;
            jobs[bit].residualsMatrix = residualsMatrixThreads[bit];
            jobs[bit].header = header;
            jobs[bit].cm = cm;
            jobs[bit].buffer = bp_buffer[bit];
            jobs[bit].buffer_size = bp_sizes[bit];

            pthread_create(&threads[bit], NULL, decode_bitplane_thread, &jobs[bit]);
        }

        for (int t = 0; t < header.bits; t++) {
            pthread_join(threads[t], NULL);
        }


        for (int t = 0; t < header.bits; t++)
        for (int z = 0; z < header.z; z++)
        for (int y = 0; y < header.y; y++)
        for (int x = 0; x < header.x; x++)
            residualsMatrix[z][y][x] |= residualsMatrixThreads[t][z][y][x];

        for (int t = 0; t < header.bits; t++) {
            free(bp_buffer[t]);  // libera los buffers de bitplane
            for (int z = 0; z < header.z; z++) {
                for (int y = 0; y < header.y; y++)
                    free(residualsMatrixThreads[t][z][y]);
                free(residualsMatrixThreads[t][z]);
            }
            free(residualsMatrixThreads[t]);
        }
        free(residualsMatrixThreads);
        free(bp_sizes);

        uint16_t min = 0, max = (1 << header.bits) - 1;
        for (int z = 0; z < header.z; z++)
            for (int y = 0; y < header.y; y++)
                for (int x = 0; x < header.x; x++) {
                    uint64_t prediction = predict(matrix, header.z, header.y, header.x, z, y, x, weights);
                    int32_t diff = unmap_difference(residualsMatrix[z][y][x], prediction, min, max);
                    matrix[z][y][x] = prediction + diff;
                }

        FILE *out = fopen(output_file, "wb");
        if (!out) { perror("open output"); return 1; }

        for (int z = 0; z < header.z; z++)
            for (int y = 0; y < header.y; y++)
                for (int x = 0; x < header.x; x++) {
                    if (header.bits == 16) {
                        uint16_t value = (uint16_t)(matrix[z][y][x] & 0xFFFF); 
                        uint8_t bytes[2];
                        if (header.endian == 'b') { bytes[0] = (value >> 8) & 0xFF; bytes[1] = value & 0xFF; }
                        else { bytes[1] = (value >> 8) & 0xFF; bytes[0] = value & 0xFF; }
                        fwrite(bytes, 1, 2, out);
                    } else {
                        uint8_t value = (uint8_t)(matrix[z][y][x] & 0xFF); 
                        fwrite(&value, 1, 1, out);
                    }
                }

        fclose(out);

        // Liberar memoria
        for (int z = 0; z < header.z; z++) {
            for (int y = 0; y < header.y; y++) {
                free(matrix[z][y]);
                free(residualsMatrix[z][y]);
            }
            free(matrix[z]);
            free(residualsMatrix[z]);
        }
        free(matrix);
        free(residualsMatrix);
    }

    return 0;
}
