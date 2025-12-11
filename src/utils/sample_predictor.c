#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

uint64_t predict(uint64_t ***inputData, int depth, int rows, int columns, int z, int y, int x, const uint64_t w[76]) {
    uint64_t prediction = 0;
    if (x >= 4 && y >= 8 && x < rows - 4) {
        for (int dy = 0; dy < 9; dy++) {
            for (int dx = 0; dx < 9; dx++) {
                if(dx == 8 && dy > 3) continue;
                prediction += inputData[z][y - 8 + dy][x - 4 + dx] * w[dy*9 + dx];
            }
        }

        return prediction / 1000;
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
    
    if (x >= 4 && y >= 8 && x < rows - 4) {
        double acc = 0.0f;
        for (int dy = 0; dy < 9; dy++) {
            for (int dx = 0; dx < 9; dx++) {
                if(dx == 8 && dy > 3) continue;
                acc += (double)inputData[z][y - 8 + dy][x - 4 + dx] * weights[dy][dx];
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
