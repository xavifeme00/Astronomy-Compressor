#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "utils/header.h"
#include "utils/reader.h"
#include "utils/sample_predictor.h"
#include "utils/arithmetic_coder.h"

double weightsMatrix[9][9] = {
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,  1./76,  1./76, 1./76, 1./76, 1./76},
    {1./76, 1./76, 1./76,  1./76,      0,      0,     0,     0,     0}
};

typedef struct {
    Header header;
    uint64_t ***matrix;
    uint64_t ***residuals;
} ImageSample;


typedef struct {
    uint64_t ***residuals;
    int z, y, x;
    int bit;
    uint16_t context;
    uint64_t max_c1, max_c2, max_c3;
    float entropy;
} EntropyJob;

void *entropy_thread(void *arg) {
    EntropyJob *job = (EntropyJob*)arg;
   
    uint32_t ****freq = NULL;
    initialize_freq(job->max_c1, job->max_c2, job->max_c3, &freq);
    if (!freq) { perror("calloc"); pthread_exit(NULL); }

    for (int zz = 0; zz < job->z; zz++)
    for (int yy = 0; yy < job->y; yy++)
    for (int xx = 0; xx < job->x; xx++) {
        uint16_t bval = (job->residuals[zz][yy][xx] >> job->bit) & 1;
        uint64_t c1, c2, c3;
        get_context(job->context, job->residuals, zz, yy, xx, job->x - 1, &c1, &c2, &c3, job->bit);
        freq[c1][c2][c3][bval]++;
    }

    // Calcular entropía
    double H_bit = 0.0;
    double total_count = (double)job->x * job->y;

    for (uint64_t c1 = 0; c1 < job->max_c1; c1++)
    for (uint64_t c2 = 0; c2 < job->max_c2; c2++)
    for (uint64_t c3 = 0; c3 < job->max_c3; c3++) {
        double f0 = freq[c1][c2][c3][0];
        double f1 = freq[c1][c2][c3][1];
        double fsum = f0 + f1;
        if (fsum == 0) continue;
        double p0 = f0 / fsum, p1 = f1 / fsum;
        double Hc = 0.0;
        if (p0 > 0) Hc -= p0 * log2(p0);
        if (p1 > 0) Hc -= p1 * log2(p1);
        H_bit += (fsum / total_count) * Hc;
    }

    job->entropy = (float)H_bit;
    free_freq(job->max_c1, job->max_c2, job->max_c3, &freq);
    pthread_exit(NULL);
}


void prediction(uint64_t ***matrix, Header header, uint64_t ***residuals, double **weights) {
    uint16_t min = 0;
        uint16_t max = (1 << header.bits) - 1;
        for (int z = 0; z < header.z; z++)
        for (int y = 0; y < header.y; y++)
        for (int x = 0; x < header.x; x++) {
            uint64_t current = matrix[z][y][x];
            uint64_t prediction = predict_train(matrix, header.z, header.x, header.y, z, y, x, weights);
            int32_t diff = (int32_t)(current - prediction);
            residuals[z][y][x] = map_difference(diff, prediction, min, max);
        }
}

float calculate_entropy(uint64_t ***residuals, Header header, uint16_t cm) {
    pthread_t *threads = malloc(header.bits * sizeof(pthread_t));
    EntropyJob *jobs = malloc(header.bits * sizeof(EntropyJob));

    uint64_t max_c1, max_c2, max_c3;
    get_max_context(cm, &max_c1, &max_c2, &max_c3);

    for (int bit = 0; bit < header.bits; bit++) {
        jobs[bit].residuals = residuals;
        jobs[bit].z = header.z; jobs[bit].y = header.y; jobs[bit].x = header.x;
        jobs[bit].bit = bit;
        jobs[bit].max_c1 = max_c1; jobs[bit].max_c2 = max_c2; jobs[bit].max_c3 = max_c3;
        jobs[bit].entropy = 0.0f;
        jobs[bit].context = cm;
        pthread_create(&threads[bit], NULL, entropy_thread, &jobs[bit]);
    }

    float total_entropy = 0.0f;
    for (int bit = 0; bit < header.bits; bit++) {
        pthread_join(threads[bit], NULL);
        total_entropy += jobs[bit].entropy;
    }

    free(threads);
    free(jobs);
    return total_entropy;
}

void numerical_gradient(ImageSample img, uint16_t cm, float entropy, double **weights, double **gradients, double eps) {

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if(i == 8 && j > 3) continue;

            double **weightsAux = malloc(9 * sizeof(double *));
            if (!weightsAux) { perror("malloc"); return; }

            for (int row = 0; row < 9; row++) {
                weightsAux[row] = malloc(9 * sizeof(double));
                if (!weightsAux[row]) { perror("malloc"); return; }
            }

            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    weightsAux[row][col] = weights[row][col];
                }
            }

            weightsAux[i][j] += eps;
            if (weightsAux[i][j] < 0.0)
                weightsAux[i][j] = 0.0;
            else if (weightsAux[i][j] > 1.0)
                weightsAux[i][j] = 1.0;

            double sum = 0.0;
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    sum += weightsAux[row][col];
                }
            }
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    weightsAux[row][col] /= sum;
                }
            }
            
            prediction(img.matrix, img.header, img.residuals, weightsAux);


            float entropy2 = calculate_entropy(img.residuals, img.header, cm);

            gradients[i][j] = (entropy2 - entropy) / eps;

            for (int row = 0; row < 9; row++) {
                free(weightsAux[row]);
            }
            free(weightsAux);
            weightsAux = NULL;
        }
    }
    
}

int load_full_dataset(const char *list_path, ImageSample **out_data, int *out_count) {
    FILE *f = fopen(list_path, "r");
    if (!f) return -1;

    char line[4096];
    int capacity = 40;
    int count = 0;

    ImageSample *data = malloc(capacity * sizeof(ImageSample));
    if (!data) return -1;

    while (fgets(line, sizeof(line), f)) {

        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        if (count == capacity) {
            capacity *= 2;
            data = realloc(data, capacity * sizeof(ImageSample));
        }

        if (header_parse_filename(line, &data[count].header) != 0) {
            fprintf(stderr, "Error parsing header: %s\n", line);
            return -1;
        }

        if (get_image_from_file(data[count].header, line, &data[count].matrix) != 0) {
            fprintf(stderr, "Error reading image: %s\n", line);
            return -1;
        }

        if (initialize_image_matrix(data[count].header.z,
                                    data[count].header.y,
                                    data[count].header.x,
                                    &data[count].residuals) != 0) {
            fprintf(stderr, "Error allocating residuals: %s\n", line);
            return -1;
        }

        count++;
    }

    fclose(f);

    *out_data = data;
    *out_count = count;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <input_dataset> <cm> <lr> <eps> <epochs>\n", argv[0]);
        return 1;
    }
    ImageSample *dataset = NULL;
    int dataset_size = 0;

    if (load_full_dataset(argv[1], &dataset, &dataset_size) != 0) {
        fprintf(stderr, "Error loading dataset\n");
        return 1;
    }
    uint16_t cm = (uint16_t)atoi(argv[2]);
    float lr = (float)atof(argv[3]);
    float num_eps = (float)atof(argv[4]);
    uint16_t epochs = (uint16_t)atoi(argv[5]);


    double **weights = malloc(9 * sizeof(double *));
    if (!weights) { perror("malloc"); return 1; }

    for (int row = 0; row < 9; row++) {
        weights[row] = malloc(9 * sizeof(double));
        if (!weights[row]) { perror("malloc"); return 1; }
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            weights[row][col] = weightsMatrix[row][col];
        }
    }
    
    double **mValue = malloc(9 * sizeof(double *));
    if (!mValue) { perror("malloc"); return 1; }

    for (int row = 0; row < 9; row++) {
        mValue[row] = malloc(9 * sizeof(double));
        if (!mValue[row]) { perror("malloc"); return 1; }
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            mValue[row][col] = 0;
        }
    }

    double **vValue = malloc(9 * sizeof(double *));
    if (!vValue) { perror("malloc"); return 1; }

    for (int row = 0; row < 9; row++) {
        vValue[row] = malloc(9 * sizeof(double));
        if (!vValue[row]) { perror("malloc"); return 1; }
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            vValue[row][col] = 0;
        }
    }

    double beta1 = 0.9;
    double beta2 = 0.999;
    double eps = 1e-8;

    for(uint16_t epoch = 0; epoch <= epochs; epoch++) {
         float avg_entropy = 0;

        for (int d = 0; d < dataset_size; d++) {
            ImageSample *img = &dataset[d];
            prediction(img->matrix, img->header, img->residuals, weights);

            float entropy = calculate_entropy(img->residuals, img->header, cm);
            avg_entropy += entropy;

            double **gradients = malloc(9 * sizeof(double *));
            if (!gradients) { perror("malloc"); return 1; }

            for (int i = 0; i < 9; i++) {
                gradients[i] = malloc(9 * sizeof(double));
                if (!gradients[i]) { perror("malloc"); return 1; }
            }

            numerical_gradient(*img, cm, entropy, weights, gradients, num_eps);


            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if(i == 8 && j > 3) continue;

                    mValue[i][j] = beta1 * mValue[i][j] + (1-beta1) * gradients[i][j];
                    vValue[i][j] = beta2 * vValue[i][j] + (1-beta2) * (gradients[i][j] * gradients[i][j]);

                    double m_hat = mValue[i][j] / (1 - pow(beta1, epoch + 1));
                    double v_hat = vValue[i][j] / (1 - pow(beta2, epoch + 1));

                    weights[i][j] -= lr * m_hat / (sqrt(v_hat) + eps);

                    if (weights[i][j] < 0.0) {
                        weights[i][j] = 0.0;
                    }
                }
            }

            double sum = 0.0;
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    sum += weights[row][col];
                }
            }
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    weights[row][col] /= sum;
                }
            }

            for (int i = 0; i < 9; i++) {
                free(gradients[i]);
            }
            free(gradients);
            gradients = NULL;
            fprintf(stdout, "\tEntropy = %f\n", entropy);
            
        }
        fprintf(stdout, "Epoch %i/%i,  Entropy = %f\n", epoch, epochs, avg_entropy / dataset_size);

    }

    uint64_t final_weights[76];


    uint16_t counter = 0;
    for (int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(i == 8 && j > 3) continue;
            final_weights[i*9 + j] = (uint64_t) round(weights[i][j]*1000);
            counter += final_weights[i*9 + j];
        }
    }

    final_weights[75] += (int)(1000 - counter);

    for (int i = 0; i < 76; i++) {
        fprintf(stdout, "%lu,", final_weights[i]);
    }
    fprintf(stdout, "\n");

}