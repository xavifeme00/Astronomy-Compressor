#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "arithmetic_coder.h"


#define PRECISION 24

#define TOP_VALUE      ((1 << PRECISION) - 1)
#define HALF           (1 << (PRECISION - 1))


/**
 * Global funcitons
 */

int initialize_freq(uint64_t max_c1, uint64_t max_c2, uint64_t max_c3, uint32_t *****freq) {
    *freq = (uint32_t ****)malloc(max_c1 * sizeof(uint32_t ***));

    if (!*freq) return 1;

    for (int i = 0; i < max_c1; i++) {
        (*freq)[i] = (uint32_t ***)malloc(max_c2 * sizeof(uint32_t **));
        if (!(*freq)[i]) {
            for (int j = 0; j < i; j++) {
                free((*freq)[j]);
            }
            free(*freq);
            return 1;
        }

        for (int j = 0; j < max_c2; j++) {
            (*freq)[i][j] = (uint32_t **)malloc(max_c3 * sizeof(uint32_t *));
            if (!(*freq)[i][j]) {
                for (int k = 0; k < j; k++) {
                    free((*freq)[i][k]);
                }
                free((*freq)[i]);
                for (int k = 0; k < i; k++) {
                    free((*freq)[k]);
                }
                free(*freq);
                return 1;
            }

            for (int k = 0; k < max_c3; k++) {
                (*freq)[i][j][k] = (uint32_t *)calloc(2, sizeof(uint32_t));
                if (!(*freq)[i][j][k]) {
                    for (int l = 0; l < k; l++) {
                        free((*freq)[i][j][l]);
                    }
                    free((*freq)[i][j]);
                    for (int l = 0; l < j; l++) {
                        for (int m = 0; m < max_c3; m++) {
                            free((*freq)[i][l][m]);
                        }
                        free((*freq)[i][l]);
                    }
                    free((*freq)[i]);
                    for (int l = 0; l < i; l++) {
                        for (int m = 0; m < max_c2; m++) {
                            for (int n = 0; n < max_c3; n++) {
                                free((*freq)[l][m][n]);
                            }
                            free((*freq)[l][m]);
                        }
                        free((*freq)[l]);
                    }
                    free(*freq);
                    return 1;
                }
            }
        }
    }
    return 0;
}

void free_freq(uint64_t max_c1, uint64_t max_c2, uint64_t max_c3, uint32_t *****freq) {
    if (!freq || !*freq) return;

    for (int i = 0; i < max_c1; i++) {
        if (!(*freq)[i]) continue;

        for (int j = 0; j < max_c2; j++) {
            if (!(*freq)[i][j]) continue;

            for (int k = 0; k < max_c3; k++) {
                free((*freq)[i][j][k]);
            }
            free((*freq)[i][j]);
        }
        free((*freq)[i]);
    }
    free(*freq);
    *freq = NULL;
}

void update_freq(ArithmeticCoder *coder, uint16_t symbol, uint64_t c1, uint64_t c2, uint64_t c3) {

    if (coder->freq[c1][c2][c3][1] == coder->context_window) {
        coder->freq_window[c1][c2][c3][0] = coder->freq[c1][c2][c3][0];   
        coder->freq_window[c1][c2][c3][1] = coder->freq[c1][c2][c3][1];   
    }

    if (symbol == 0) {
        coder->freq[c1][c2][c3][0] += 1;
    }
    coder->freq[c1][c2][c3][1] += 1;

    if (coder->freq[c1][c2][c3][1] >= coder->context_window * 2 - 2) {
        coder->freq[c1][c2][c3][0] -= (coder->freq_window[c1][c2][c3][0] - 1);
        coder->freq[c1][c2][c3][1] -= (coder->freq_window[c1][c2][c3][1] - 2);
    }
}


/**
 * ENCODER FUNCTIONS
 */
static void write_word(ArithmeticCoder *coder) {


    coder->buffer[coder->buffer_index++] = (coder->low >> 16) & 255;
    coder->buffer[coder->buffer_index++] = (coder->low >> 8) & 255;
    coder->buffer[coder->buffer_index++] = (coder->low) & 255;

}

int init_coder(ArithmeticCoder *coder, int context, int context_window, uint8_t *buffer, size_t max_size) {
    coder->context = context;
    coder->context_window = context_window;

    coder->low = 0;
    coder->interval = TOP_VALUE;

    coder->buffer = buffer;
    coder->buffer_size = max_size;
    coder->buffer_index = 0;

    uint64_t max_c1, max_c2, max_c3;
    get_max_context(coder->context, &max_c1, &max_c2, &max_c3);

    initialize_freq(max_c1, max_c2, max_c3, &coder->freq);
    initialize_freq(max_c1, max_c2, max_c3, &coder->freq_window);

    // Initialize frequencies
    for (int c1 = 0; c1 < max_c1; c1++)
        for (int c2 = 0; c2 < max_c2; c2++)
            for (int c3 = 0; c3 < max_c3; c3++) {
                    coder->freq[c1][c2][c3][0] = 1;
                    coder->freq[c1][c2][c3][1] = 2;
                    coder->freq_window[c1][c2][c3][0] = 0;
                    coder->freq_window[c1][c2][c3][1] = 0;
            }

    return 0;
}


int encode_symbol(ArithmeticCoder *coder, uint16_t symbol, uint64_t c1, uint64_t c2, uint64_t c3, int bit, int y, int x) {
    uint32_t tmp = (uint32_t)(((uint64_t)coder->interval * coder->freq[c1][c2][c3][0]) / coder->freq[c1][c2][c3][1]);


    if (symbol == 0) {
        coder->interval = tmp;

    } else {
        coder->low += tmp + 1;
        coder->interval -= (tmp + 1);
    }


    if (coder->interval == 0) {
        // send interval
        write_word(coder);
        coder->low = 0;
        coder->interval = TOP_VALUE;
    }

    return 0;
}

size_t finalize_encoding(ArithmeticCoder *coder) {

    uint64_t interval1 = 0;
    uint64_t interval2 = 0;
    int bits = PRECISION;
    uint64_t intervalMax = coder->low + coder->interval;

    while (((interval1 < coder->low) || (interval1 > intervalMax)) &&
            ((interval2 < coder->low) || (interval2 > intervalMax)))
    {
        bits--;
        interval1 |= ((uint64_t) 1 << bits) & coder->low;
        interval2 |= ((uint64_t) 1 << bits) & intervalMax;
    }
    if((interval1 >= coder->low) && (interval1 <= intervalMax)) {
        coder->interval = interval1;
    } else {
        coder->interval = interval2;
    }

    // Send interval
    write_word(coder);
    uint64_t max_c1,max_c2,max_c3;
    get_max_context(coder->context, &max_c1,&max_c2,&max_c3);
    free_freq(max_c1,max_c2,max_c3,&coder->freq);

    return coder->buffer_index;
}

/**
 * DECODER FUNCTIONS
 */
static void read_word(ArithmeticCoder *coder) {

    coder->word_buffer = 0;
    coder->word_buffer |= (uint32_t)(coder->buffer[coder->buffer_index++] << 16);
    coder->word_buffer |= (uint32_t)(coder->buffer[coder->buffer_index++] << 8);
    coder->word_buffer |= (uint32_t)(coder->buffer[coder->buffer_index++]);

}

int init_decoder(ArithmeticCoder *coder, int context,int context_window, uint8_t *buffer, size_t buffer_size) {
    coder->context = context;
    coder->context_window = context_window;


    coder->buffer = buffer;
    coder->buffer_size = buffer_size;
    coder->buffer_index = 0;

    coder->low = 0;
    coder->interval = 0;
    coder->word_buffer = 0;

    uint64_t max_c1,max_c2,max_c3;
    get_max_context(coder->context, &max_c1,&max_c2,&max_c3);
    initialize_freq(max_c1,max_c2,max_c3, &coder->freq);
    initialize_freq(max_c1,max_c2,max_c3, &coder->freq_window);

    for (int c1 = 0; c1 < max_c1; c1++)
        for (int c2 = 0; c2 < max_c2; c2++)
            for (int c3 = 0; c3 < max_c3; c3++) {
                    coder->freq[c1][c2][c3][0] = 1;
                    coder->freq[c1][c2][c3][1] = 2;
                    coder->freq_window[c1][c2][c3][0] = 0;
                    coder->freq_window[c1][c2][c3][1] = 0;
            }

    return 0;
}

uint16_t decode_symbol(ArithmeticCoder *coder, uint64_t c1, uint64_t c2, uint64_t c3, int bit, int y, int x) {
    if (coder->interval == 0) {
        read_word(coder);
        coder->low = 0;
        coder->interval = TOP_VALUE;
    } 


    uint32_t tmp = (uint32_t)(((uint64_t)coder->interval * coder->freq[c1][c2][c3][0]) / coder->freq[c1][c2][c3][1]);

    uint32_t mid = coder->low + tmp;



    if (coder->word_buffer > coder->low + tmp) {
        coder->low = mid+1;
        coder->interval -= (tmp + 1);
        return 1;
    } else {
        coder->interval = tmp;
        return 0;
    }
}

void finalize_decoding(ArithmeticCoder *coder) {
    uint64_t max_c1, max_c2, max_c3;
    get_max_context(coder->context, &max_c1, &max_c2, &max_c3);
    free_freq(max_c1, max_c2, max_c3, &coder->freq);
}

