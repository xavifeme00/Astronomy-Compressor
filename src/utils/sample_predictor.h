#ifndef SAMPLE_PREDICTOR_H
#define SAMPLE_PREDICTOR_H

#include <stdint.h>

#define BUFFER_SIZE 1024

int16_t map_difference(int residual, int prediction, int min, int max);
int     unmap_difference(uint16_t mapped_difference, int prediction, int min, int max);
uint64_t predict(uint64_t ***inputData, int depth, int rows, int columns, int z, int y, int x, const uint64_t w[32]);
uint64_t predict_train(uint64_t ***inputData, int depth, int rows, int columns, int z, int y, int x, double **weights);

#endif
