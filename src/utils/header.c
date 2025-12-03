#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int header_parse_filename(const char *filename, Header *header) {
    char *last_dash = strrchr(filename, '-');
    if (!last_dash || last_dash < filename + 3) {
        fprintf(stderr, "Error: Invalid filename format.\n");
        return 1;
    }

    char *format_start = last_dash - 4;
    if (*format_start != 'u' && *format_start != 's') {
        format_start = last_dash - 5;
        if (*format_start != 'u' && *format_start != 's') {
            fprintf(stderr, "Error: Could not find format start.\n");
            return 1;
        }
    }

    char s_char, endian_char;
    int bits;
    int n = sscanf(format_start, "%c%d%ce-%dx%dx%d",
                   &s_char, &bits, &endian_char,
                   &header->z, &header->y, &header->x);
    if (n != 6) {
        fprintf(stderr, "Error: Could not parse filename '%s'.\n", filename);
        return 1;
    }

    header->signed_char = s_char;
    header->bits = bits;
    header->endian = endian_char;

    return 0;
}

int header_parse_compressed(FILE *in, Header *header, size_t **bp_sizes_out) {
    if (fread(header, sizeof(Header), 1, in) != 1) {
        fprintf(stderr, "Error reading header from compressed file.\n");
        return 1;
    }

    int bits = header->bits;
    size_t *bp_sizes = calloc(bits, sizeof(size_t));
    if (!bp_sizes) {
        fprintf(stderr, "Memory allocation error.\n");
        return 1;
    }

    for (int bit = bits - 1; bit >= 0; bit--) {
        if (fread(&bp_sizes[bit], sizeof(size_t), 1, in) != 1) {
            fprintf(stderr, "Error reading bitplane size.\n");
            free(bp_sizes);
            return 1;
        }
    }

    *bp_sizes_out = bp_sizes;

    return 0;
}
