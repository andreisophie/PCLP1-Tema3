// Copyright Andrei Maruntis 2022, 313CA

#ifndef __IMAGETOOLS_C__
#define __IMAGETOOLS_C__

#include "image.h"
#include "constants.h"

void swap_ints(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

int select_image(image_t *p_image, int *x1, int *y1, int *x2, int *y2)
{
	if (*x1 > *x2)
		swap_ints(x1, x2);
	if (*y1 > *y2)
		swap_ints(y1, y2);

	if (*x1 < 0 || *y1 < 0 || *x2 > p_image->cols ||
		*y2 > p_image->lines || *x1 == *x2 || *y1 == *y2)
		return -1;

	p_image->x1 = *x1;
	p_image->y1 = *y1;
	p_image->x2 = *x2;
	p_image->y2 = *y2;

	return 0;
}

//functie care verifica daca e selectata intreaga imagine
int entire_image(image_t *p_image)
{
	return p_image->x1 == 0 && p_image->y1 == 0 &&
		p_image->x2 == p_image->cols && p_image->y2 == p_image->lines;
}

int rotate_image(image_t *p_image)
{
	// aloc structura image_aux
	image_t image_aux;
	image_aux.type = p_image->type;
	image_aux.lines = p_image->x2 - p_image->x1;
	image_aux.cols = p_image->y2 - p_image->y1;

	if (image_aux.lines != image_aux.cols && !entire_image(p_image))
		return 1;

	image_aux.x1 = 0;
	image_aux.y1 = 0;
	image_aux.x2 = image_aux.cols;
	image_aux.y2 = image_aux.lines;
	for (int k = 0; k < image_aux.type; k++) {
		image_aux.matrix[k] = alloc_matrix(image_aux.lines, image_aux.cols);
		if (!image_aux.matrix[k])
			return -1;
	}

	// bag in image_aux pixelii selectati din memorie, rotiti cu 90 grade
	for (int i = 0; i < image_aux.lines; i++) {
		for (int j = 0; j < image_aux.cols; j++) {
			for (int k = 0; k < image_aux.type; k++)
				image_aux.matrix[k][i][j] =
					p_image->matrix[k][p_image->y1 + image_aux.cols - j - 1]
						[p_image->x1 + i];
		}
	}
	//copiez in memorie pixelii din aux
	if (entire_image(p_image)) {
		free_image(p_image);
		*p_image = image_aux;
	} else {
		for (int i = 0; i < image_aux.lines; i++) {
			for (int j = 0; j < image_aux.cols; j++) {
				for (int k = 0; k < image_aux.type; k++)
					p_image->matrix[k][i + p_image->y1][j + p_image->x1] =
						image_aux.matrix[k][i][j];
			}
		}
		free_image(&image_aux);
	}
	return 0;
}

int crop_image(image_t *p_image)
{
	// aloc memorie pentru image_aux
	image_t image_aux;
	image_aux.type = p_image->type;
	image_aux.lines = p_image->y2 - p_image->y1;
	image_aux.cols = p_image->x2 - p_image->x1;
	image_aux.x1 = 0;
	image_aux.y1 = 0;
	image_aux.x2 = image_aux.cols;
	image_aux.y2 = image_aux.lines;

	for (int k = 0; k < image_aux.type; k++) {
		image_aux.matrix[k] = alloc_matrix(image_aux.lines, image_aux.cols);
		if (!image_aux.matrix[k])
			return -1;
	}

	// copiez pixel cu pixel imaginea din selectie in aux
	for (int i = 0; i < image_aux.lines; i++) {
		for (int j = 0; j < image_aux.cols; j++) {
			for (int k = 0; k < image_aux.type; k++)
				image_aux.matrix[k][i][j] =
					p_image->matrix[k][p_image->y1 + i][p_image->x1 + j];
		}
	}

	// eliberez memoria pentru imaginea veche
	free_image(p_image);
	*p_image = image_aux;

	return 0;
}

double calculate_filter(image_t *p_image, int k, int i, int j,
						const double kernel[KERNEL_SIZE][KERNEL_SIZE])
{
	// returneaza pixelul nemodificat daca se afla pe marginea imaginii
	if (i == 0 || j == 0 || i == p_image->lines - 1 || j == p_image->cols - 1)
		return p_image->matrix[k][i][j];

	// calculeaza valoarea pixelului altfel
	double pxl_value = 0;
	for (int  u = 0; u < LAYERS; u++) {
		for (int v = 0; v < LAYERS; v++)
			pxl_value +=
				p_image->matrix[k][i + u - 1][j + v - 1] * kernel[u][v];
	}

	// clamp daca este cazul
	if (pxl_value <= 0)
		return 0;
	if (pxl_value >= COLOR_INTENSITY)
		return COLOR_INTENSITY;

	return pxl_value;
}

int apply_filter(image_t *p_image,
				 const double kernel[KERNEL_SIZE][KERNEL_SIZE])
{
	// returneaza -1 daca matricea este alb-negru
	if (p_image->type != 3)
		return 1;

	// aloc memorie pentru matricea auxiliara in care pun pixelii filtrati
	double **mat_aux[LAYERS];

	for (int k = 0; k < p_image->type; k++) {
		mat_aux[k] = alloc_matrix(p_image->lines, p_image->cols);

		if (!mat_aux[k])
			return -1;
	}

	// calculez valoarea fiecarui pixel
	for (int i = p_image->y1; i < p_image->y2; i++) {
		for (int j = p_image->x1; j < p_image->x2; j++) {
			for (int k = 0; k < p_image->type; k++)
				mat_aux[k][i - p_image->y1][j - p_image->x1] =
					calculate_filter(p_image, k, i, j, kernel);
		}
	}

	// copiez pixel cu pixel inapoi in memorie, in zona selectata
	for (int i = p_image->y1; i < p_image->y2; i++) {
		for (int j = p_image->x1; j < p_image->x2; j++) {
			for (int k = 0; k < p_image->type; k++)
				p_image->matrix[k][i][j] =
					mat_aux[k][i - p_image->y1][j - p_image->x1];
		}
	}

	// eliberez memoria alocata lui mat_aux
	for (int k = 0; k < p_image->type; k++) {
		for (int i = 0; i < p_image->lines; i++)
			free(mat_aux[k][i]);
		free(mat_aux[k]);
	}

	return 0;
}

#endif
