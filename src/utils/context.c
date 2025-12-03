#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "context.h"


void get_context(int context, uint64_t ***image, uint64_t z, uint64_t y, uint64_t x, uint64_t max_x, uint64_t *c1, uint64_t *c2, uint64_t *c3, uint16_t bit) {

    if (context == NO_CONTEXT || (y == 0 && x == 0)) {
        *c1 = 0;
        *c2 = 0;
        *c3 = 0;
    } 
    else if (context == CONTEXT_UP) {
        *c1 = 0;
        *c2 = 0;
        *c3 = y == 0 ? (image[z][y][x-1] >> bit) & 1 : (image[z][y-1][x] >> bit) & 1;
    }
    else if (context == CONTEXT_LEFT) {
        *c1 = 0;
        *c2 = 0;
        *c3 = x == 0 ? (image[z][y-1][x] >> bit) & 1 : (image[z][y][x-1] >> bit) & 1;
    }
    else if (context == CONTEXT_UP_LEFT ) {
        *c1 = 0;
        *c2 = y == 0 ? (image[z][y][x-1] >> bit) & 1 : (image[z][y-1][x] >> bit) & 1;
        *c3 = x == 0 ? (image[z][y-1][x] >> bit) & 1 : (image[z][y][x-1] >> bit) & 1;
    }
    else if (context == CONTEXT_UP_LEFT_DIAGONAL) {
        *c1 = (x == 0 || y == 0) ? 0 : (image[z][y-1][x-1] >> bit) & 1;
        *c2 = y == 0 ? (image[z][y][x-1] >> bit) & 1 : (image[z][y-1][x] >> bit) & 1;
        *c3 = x == 0 ? (image[z][y-1][x] >> bit) & 1 : (image[z][y][x-1] >> bit) & 1;
    }
   
}

void get_max_context(int context, uint64_t *max_c1, uint64_t *max_c2, uint64_t *max_c3) {
    if (context == NO_CONTEXT) {
        *max_c1 = 1;
        *max_c2 = 1;
        *max_c3 = 1;
    } else if (context == CONTEXT_LEFT || context == CONTEXT_UP) {
        *max_c1 = 1;
        *max_c2 = 1;
        *max_c3 = 2;
    } else if (context == CONTEXT_UP_LEFT) {
        *max_c1 = 1;
        *max_c2 = 2;
        *max_c3 = 2;
    } else if (context == CONTEXT_UP_LEFT_DIAGONAL) {
        *max_c1 = 2;
        *max_c2 = 2;
        *max_c3 = 2;
    }
}