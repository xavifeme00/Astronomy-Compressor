#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int16_t map_difference(int residual, int prediction, int min, int max) {
    int32_t theta = (prediction - min < max - prediction) ? (prediction - min) : (max - prediction);
    uint16_t mappedDiff = 0;
    if (residual >= 0 && residual <= theta) {
        mappedDiff = residual << 1;
    } else if (residual >= -theta && residual < 0) {
        mappedDiff = (abs(residual) << 1) - 1;

    } else {
        mappedDiff = theta + abs(residual);
    }

    return mappedDiff;
}

int unmap_difference(uint16_t mapped_difference, int prediction, int min, int max) {
    uint32_t left_interval = (prediction - min <= max - prediction);
    int32_t theta = left_interval ? (prediction - min) : (max - prediction);
    int prediction_error;

    if (mapped_difference <= (theta << 1)) {
        if ((mapped_difference & 1) == 0) {
            prediction_error = mapped_difference >> 1;
        } else {
            prediction_error = -((mapped_difference + 1) >> 1);
        }
    } else {
        if (left_interval) {
            prediction_error = mapped_difference - theta;
        } else {
            prediction_error = theta - mapped_difference;
        }
    }

    return prediction_error;
}

uint64_t predict(uint64_t ***inputData, int depth, int rows, int columns, int z, int y, int x, const uint64_t w[32]) {
    uint64_t prediction = 0;
    if (x >= 4 && y >= 4 && x < rows - 2) {
        prediction = (
        inputData[z][y-4][x-4] * w[ 0] + inputData[z][y-4][x-3] * w[ 1] + inputData[z][y-4][x-2] * w[ 2] + inputData[z][y-4][x-1] * w[ 3] + inputData[z][y-4][x-0] * w[ 4] + inputData[z][y-4][x+1] * w[ 5] + inputData[z][y-4][x+2] * w[ 6] +
        inputData[z][y-3][x-4] * w[ 7] + inputData[z][y-3][x-3] * w[ 8] + inputData[z][y-3][x-2] * w[ 9] + inputData[z][y-3][x-1] * w[10] + inputData[z][y-3][x-0] * w[11] + inputData[z][y-3][x+1] * w[12] + inputData[z][y-3][x+2] * w[13] +
        inputData[z][y-2][x-4] * w[14] + inputData[z][y-2][x-3] * w[15] + inputData[z][y-2][x-2] * w[16] + inputData[z][y-2][x-1] * w[17] + inputData[z][y-2][x-0] * w[18] + inputData[z][y-2][x+1] * w[19] + inputData[z][y-2][x+2] * w[20] +
        inputData[z][y-1][x-4] * w[21] + inputData[z][y-1][x-3] * w[22] + inputData[z][y-1][x-2] * w[23] + inputData[z][y-1][x-1] * w[24] + inputData[z][y-1][x-0] * w[25] + inputData[z][y-1][x+1] * w[26] + inputData[z][y-1][x+2] * w[27] +
        inputData[z][y-0][x-4] * w[28] + inputData[z][y-0][x-3] * w[29] + inputData[z][y-0][x-2] * w[30] + inputData[z][y-0][x-1] * w[31]
        ) / 1000;

        return prediction;
    }

    if (x >= 3 && y >= 3 && x < rows - 1) {
        prediction = (
        inputData[z][y-3][x-3] + inputData[z][y-3][x-2]        + inputData[z][y-3][x-1]      + inputData[z][y-3][x]        +
        inputData[z][y-2][x-3] + inputData[z][y-2][x-2]        + inputData[z][y-2][x-1]      + inputData[z][y-2][x]        + 
        inputData[z][y-1][x-3] + inputData[z][y-1][x-2]        + inputData[z][y-1][x-1]      + inputData[z][y-1][x] * 2    + inputData[z][y-1][x+1] * 2 +
        inputData[z][y-0][x-3] + inputData[z][y-0][x-2] * 2    + inputData[z][y-0][x-1] * 3
        ) / 21;
        
        return prediction;
    }

    if (x >= 3 && y >= 2 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      (inputData[z][y][x-2] << 1) + inputData[z][y][x-3] +
                      inputData[z][y-1][x-1] + inputData[z][y-1][x-2] +
                      inputData[z][y-1][x-3] + inputData[z][y-1][x+1] +
                      inputData[z][y-2][x] + inputData[z][y-2][x-1] +
                      inputData[z][y-2][x-2] + inputData[z][y-2][x-3] +
                      inputData[z][y-1][x+1]) / 17;
        return prediction;
    }

    if (x >= 2 && y >= 2 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y][x-2] + inputData[z][y-1][x-1] +
                      inputData[z][y-1][x-2] + inputData[z][y-1][x+1] +
                      inputData[z][y-2][x] + inputData[z][y-2][x-1] +
                      inputData[z][y-2][x-2] + inputData[z][y-1][x+1]) / 13;
        return prediction;
    }

    if (x >= 2 && y >= 1 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y][x-2] + inputData[z][y-1][x-1] +
                      inputData[z][y-1][x-2] + inputData[z][y-1][x+1]) / 9;
        return prediction;
    }

    if (x >= 1 && y >= 1 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y-1][x-1] + inputData[z][y-1][x+1]) / 7;
        return prediction;
    }

    if (x >= 2 && y >= 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      (inputData[z][y][x-2] << 1) + inputData[z][y-1][x-1]) / 8;
        return prediction;
    }

    if (x >= 1 && y >= 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y-1][x-1]) / 6;
        return prediction;
    }

    if (x == 0 && y >= 1) {
        prediction = inputData[z][y-1][x];
        return prediction;
    }

    if (y == 0 && x >= 2) {
        prediction = (inputData[z][y][x-1] + inputData[z][y][x-2]) >> 1;
        return prediction;
    }

    if (y == 0 && x >= 1) {
        prediction = inputData[z][y][x-1];
        return prediction;
    }

    if (x == 0 && y == 0) {
        prediction = 0;
        return prediction;
    }

    return prediction;
}

uint64_t predict_train(uint64_t ***inputData, int depth, int rows, int columns, int z, int y, int x, double **weights) {
    uint64_t prediction = 0;
    
    if (x >= 4 && y >= 4 && x < rows - 2) {
        double acc = 0.0f;
        for (int dy = 0; dy < 5; dy++) {
            for (int dx = 0; dx < 7; dx++) {
                acc += (double)inputData[z][y - 4 + dy][x - 4 + dx] * weights[dy][dx];
            }
        }

        return (uint64_t)acc;
    }

    if (x >= 3 && y >= 3 && x < rows - 1) {
        prediction = (
        inputData[z][y-3][x-3] + inputData[z][y-3][x-2]        + inputData[z][y-3][x-1]      + inputData[z][y-3][x]        +
        inputData[z][y-2][x-3] + inputData[z][y-2][x-2]        + inputData[z][y-2][x-1]      + inputData[z][y-2][x]        + 
        inputData[z][y-1][x-3] + inputData[z][y-1][x-2]        + inputData[z][y-1][x-1]      + inputData[z][y-1][x] * 2    + inputData[z][y-1][x+1] * 2 +
        inputData[z][y-0][x-3] + inputData[z][y-0][x-2] * 2    + inputData[z][y-0][x-1] * 3
        ) / 21;
        
        return prediction;
    }

    if (x >= 3 && y >= 2 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      (inputData[z][y][x-2] << 1) + inputData[z][y][x-3] +
                      inputData[z][y-1][x-1] + inputData[z][y-1][x-2] +
                      inputData[z][y-1][x-3] + inputData[z][y-1][x+1] +
                      inputData[z][y-2][x] + inputData[z][y-2][x-1] +
                      inputData[z][y-2][x-2] + inputData[z][y-2][x-3] +
                      inputData[z][y-1][x+1]) / 17;
        return prediction;
    }

    if (x >= 2 && y >= 2 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y][x-2] + inputData[z][y-1][x-1] +
                      inputData[z][y-1][x-2] + inputData[z][y-1][x+1] +
                      inputData[z][y-2][x] + inputData[z][y-2][x-1] +
                      inputData[z][y-2][x-2] + inputData[z][y-1][x+1]) / 13;
        return prediction;
    }

    if (x >= 2 && y >= 1 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y][x-2] + inputData[z][y-1][x-1] +
                      inputData[z][y-1][x-2] + inputData[z][y-1][x+1]) / 9;
        return prediction;
    }

    if (x >= 1 && y >= 1 && x < rows - 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y-1][x-1] + inputData[z][y-1][x+1]) / 7;
        return prediction;
    }

    if (x >= 2 && y >= 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      (inputData[z][y][x-2] << 1) + inputData[z][y-1][x-1]) / 8;
        return prediction;
    }

    if (x >= 1 && y >= 1) {
        prediction = ((inputData[z][y-1][x] << 1) + inputData[z][y][x-1] * 3 +
                      inputData[z][y-1][x-1]) / 6;
        return prediction;
    }

    if (x == 0 && y >= 1) {
        prediction = inputData[z][y-1][x];
        return prediction;
    }

    if (y == 0 && x >= 2) {
        prediction = (inputData[z][y][x-1] + inputData[z][y][x-2]) >> 1;
        return prediction;
    }

    if (y == 0 && x >= 1) {
        prediction = inputData[z][y][x-1];
        return prediction;
    }

    if (x == 0 && y == 0) {
        prediction = 0;
        return prediction;
    }

    return prediction;
}
