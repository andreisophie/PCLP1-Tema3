// Copyright Andrei Maruntis 2022, 313CA

#ifndef __IMAGEIO_H__
#define __IMAGEIO_H__

#include <stdio.h>
#include "image.h"
#include "imageio.c"

// coding style error??
void skip_comments(FILE * fin);

void read_image(image_t *p_image, FILE *fin);

unsigned char round_pxl(double x);

void write_image(image_t *p_image, FILE *fout, int is_binary);

#endif
