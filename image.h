// Copyright Andrei Maruntis 2022, 313CA

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdlib.h>
#include "image.c"

double **alloc_matrix(int lines, int cols);

void free_image(image_t *p_image);

#endif
