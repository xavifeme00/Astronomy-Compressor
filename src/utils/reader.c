#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 

#include "header.h"
#include "reader.h"

int initialize_image_matrix(int z, int y, int x, uint64_t ****matrix) {
    *matrix = (uint64_t ***)malloc(z * sizeof(uint64_t **)); 
    if (!*matrix) return 1;

    for (int i = 0; i < z; i++) {
        (*matrix)[i] = (uint64_t **)malloc(y * sizeof(uint64_t *));
        if (!(*matrix)[i]) {
            for (int j = 0; j < i; j++) {
                free((*matrix)[j]);
            }
            free(*matrix);
            return 1;
        }
        for (int j = 0; j < y; j++) {
            (*matrix)[i][j] = (uint64_t *)calloc(x, sizeof(uint64_t));
            if (!(*matrix)[i][j]) {
                for (int k = 0; k < j; k++) {
                    free((*matrix)[i][k]);
                }
                free((*matrix)[i]);
                for (int k = 0; k < i; k++) {
                    free((*matrix)[k]);
                }
                free(*matrix);
                return 1;
            }
        }
    }
    return 0;
}

void free_image(uint64_t ***matrix, Header header) {
    if (matrix == NULL) return;

    for (int i = 0; i < header.z; i++) {
        if (matrix[i] == NULL) continue;

        for (int j = 0; j < header.y; j++) {
            if (matrix[i][j] != NULL) {
                free(matrix[i][j]);
            }
        }

        free(matrix[i]);
    }
    free(matrix);
    matrix = NULL;
}



int get_image_from_file(const Header header, const char *filename, uint64_t ****matrix)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error loading file\n");
        return 1;
    }

    uint8_t buffer[BUFFER_SIZE];
    size_t bytes_read;

    // Inicializa la matriz
    if (initialize_image_matrix(header.z, header.y, header.x, matrix) != 0) {
        perror("Error initializing the matrix\n");
        fclose(file);
        return 1;
    }

    int z_index = 0, y_index = 0, x_index = 0;
    uint16_t result;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read;) {

            if (header.bits > 8) {

                if (i + 1 >= bytes_read) break;

                if (header.endian == 'b') {  // big endian
                    result = (uint16_t)(buffer[i + 1] | (buffer[i] << 8));
                } else {                     // little endian
                    result = (uint16_t)(buffer[i] | (buffer[i + 1] << 8));
                }

                i += 2;

            } else {
                result = buffer[i];
                i++;
            }

            (*matrix)[z_index][y_index][x_index] = (uint64_t)result;
            
            x_index++;
            if (x_index >= header.x) {
                x_index = 0;
                y_index++;
            }

            if (y_index >= header.y) {
                y_index = 0;
                z_index++;
            }

            if (z_index >= header.z) {
                fclose(file);
                return 0; 
            }
        }
    }

    fclose(file);

    if (z_index < header.z) {
        perror("Error filling the matrix (unexpected EOF)\n");
        return 1;
    }

    return 0;
}