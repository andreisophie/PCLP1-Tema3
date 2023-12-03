// Copyright Andrei Maruntis 2022, 313CA

#ifndef __IMAGETOOLS_H__
#define __IMAGETOOLS_H__

#include "image.h"
#include "imagetools.c"

void swap_ints(int *a, int *b);

int select_image(image_t *p_image, int *x1, int *y1, int *x2, int *y2);

int entire_image(image_t *p_image);

int rotate_image(image_t *p_image);

int crop_image(image_t *p_image);

double calculate_filter(image_t *p_image, int k, int i, int j,
						const double kernel[KERNEL_SIZE][KERNEL_SIZE]);

int apply_filter(image_t *p_image,
				 const double kernel[KERNEL_SIZE][KERNEL_SIZE]);

#endif
