#ifndef READER_H
#define READER_H

#include <stdint.h>
#include "header.h"

#define BUFFER_SIZE 1024

int initialize_image_matrix(int z, int y, int x, uint64_t ****matrix);
void free_image(uint64_t ***matrix, Header header);
int get_image_from_file(const Header header, const char *filename, uint64_t ****matrix);

#endif