// Copyright Andrei Maruntis 2022, 313CA

#ifndef __IMAGE_C__
#define __IMAGE_C__

#include <stdlib.h>
#include "constants.h"

// definitia structurii image
// retine aproape toate datele nceesarii prelucrarii imagini in orice moment,
// anume tipul imaginii (color sau alb-negru), nr de linii si de coloane,
// matricea de pixeli si pixelii selectati
typedef struct image_t {
	int type;
	int lines, cols;
	double **matrix[LAYERS];
	int x1, y1, x2, y2;
} image_t;

// functie care aloca memorie pentru o matrice fiindu-i date dimensiunile
double **alloc_matrix(int lines, int cols)
{
	double **matrix;
	matrix = (double **)malloc(lines * sizeof(double *));
	if (!matrix)
		return NULL;
	for (int i = 0; i < lines; i++) {
		matrix[i] = (double *)malloc(cols * sizeof(double));
		if (!matrix[i]) {
			for (int k = 0 ; k < i; k++)
				free(matrix[k]);
			free(matrix);
			return NULL;
		}
	}
	return matrix;
}

// functie care elibereaza memoria matricei dintr-o structura imagine si
// reseteaza celelalte date la 0
void free_image(image_t *p_image)
{
	for (int k = 0; k < p_image->type; k++) {
		for (int i = 0; i < p_image->lines; i++)
			free(p_image->matrix[k][i]);
		free(p_image->matrix[k]);
		p_image->matrix[k] = NULL;
	}
	p_image->type = 0;
	p_image->lines = 0;
	p_image->cols = 0;
	p_image->x1 = 0;
	p_image->y1 = 0;
	p_image->x2 = 0;
	p_image->y2 = 0;
}

#endif
