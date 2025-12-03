#ifndef ARITHMETIC_CODER_H
#define ARITHMETIC_CODER_H

#include <stdint.h>
#include "context.h"

typedef struct ArithmeticCoder {
    int context;
    int context_window;

    // Arithmetic coding range
    uint32_t low;
    uint32_t interval;

    // General Buffer
    uint8_t *buffer;
    size_t buffer_size;
    size_t buffer_index;

    // Decoding buffer word
    uint32_t word_buffer;

    // Frequencies
    uint32_t ****freq;
    uint32_t ****freq_window;

} ArithmeticCoder;

// Frequency management
int initialize_freq(uint64_t max_c1, uint64_t max_c2, uint64_t max_c3, uint32_t *****freq);
void free_freq(uint64_t max_c1, uint64_t max_c2, uint64_t max_c3, uint32_t *****freq);
void update_freq(ArithmeticCoder *coder, uint16_t symbol, uint64_t c1, uint64_t c2, uint64_t c3);

// Encoder
int init_coder(ArithmeticCoder *coder, int context, int context_window, uint8_t *buffer, size_t max_size);
int encode_symbol(ArithmeticCoder *coder, uint16_t symbol, uint64_t c1, uint64_t c2, uint64_t c3, int bit, int y, int x);
size_t finalize_encoding(ArithmeticCoder *coder);

// Decoder
int init_decoder(ArithmeticCoder *coder, int context,int context_window, uint8_t *buffer, size_t buffer_size);
uint16_t decode_symbol(ArithmeticCoder *coder, uint64_t c1, uint64_t c2, uint64_t c3, int bit, int y, int x);
void finalize_decoding(ArithmeticCoder *coder);

#endif
