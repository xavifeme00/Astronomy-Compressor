#include <stdint.h>
#ifndef CONTEXT_H
#define CONTEXT_H

typedef enum {
    NO_CONTEXT,
    CONTEXT_LEFT,
    CONTEXT_UP,
    CONTEXT_UP_LEFT,
    CONTEXT_UP_LEFT_DIAGONAL
} ContextType;


void get_context(int context, uint64_t ***image, uint64_t z, uint64_t y, uint64_t x, uint64_t max_x, uint64_t *c1, uint64_t *c2, uint64_t *c3, uint16_t bit);
void get_max_context(int context, uint64_t *max_c1, uint64_t *max_c2, uint64_t *max_c3);

#endif