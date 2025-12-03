#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>

#define RAW 0
#define RESIDUAL 1
#define BITSTREAM 2

#define COMPRESS 0
#define DECOMPRESS 1 

typedef struct {
    char signed_char;
    int bits;
    char endian;
    int z, x, y;
} Header;

int header_parse_filename(const char *filename, Header *header);
int header_parse_compressed(FILE *in, Header *header, size_t **bp_sizes_out);
void print_header(const Header *header);

#endif
