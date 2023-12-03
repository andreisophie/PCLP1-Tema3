// Copyright Andrei Maruntis 2022, 313CA

#ifndef __IMAGEIO_C__
#define __IMAGEIO_C__

#include <stdio.h>
#include "image.h"
#include "constants.h"

// functie care sare comentariile din fisierele ascii
void skip_comments(FILE *fin)
{
	char letter;
	char row[CHAR_MAX];
	fscanf(fin, "%c", &letter);
	while (letter == '#' || letter == '\n' ||
		   letter == '\r' || letter == ' ') {
		if (letter == '#') {
			fgets(row, CHAR_MAX, fin);
		} else {
			if (feof(fin))
				return;
		}
		fscanf(fin, "%c", &letter);
	}
	fseek(fin, -1, SEEK_CUR);
}

void read_image(image_t *p_image, FILE *fin)
{
	char format[5];
	int type, is_binary;
	skip_comments(fin);
	fscanf(fin, "%s", format);
	switch (format[1]) {
	case ('2'):
		type = 1;
		is_binary = 0;
		break;
	case ('3'):
		type = 3;
		is_binary = 0;
		break;
	case('5'):
		type = 1;
		is_binary = 1;
		break;
	case ('6'):
		type = 3;
		is_binary = 1;
		break;
	}
	int lines, cols, color_intensity;
	skip_comments(fin);
	fscanf(fin, "%d %d", &cols, &lines);
	skip_comments(fin);
	fscanf(fin, "%d", &color_intensity);
	skip_comments(fin);
	if (color_intensity != COLOR_INTENSITY)
		return;

	double **mat_aux[LAYERS];

	for (int k = 0; k < type; k++) {
		mat_aux[k] = alloc_matrix(lines, cols);

		if (!mat_aux[k])
			return;
	}

	if (p_image->type != 0)
		free_image(p_image);

	unsigned char aux;
	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < type; k++) {
				if (!is_binary) {
					skip_comments(fin);
					fscanf(fin, "%hhu", &aux);
					mat_aux[k][i][j] = (double)aux;
				} else {
					fread(&aux, sizeof(unsigned char), 1, fin);
					mat_aux[k][i][j] = (double)aux;
				}
			}
		}
	}

	p_image->type = type;
	p_image->lines = lines;
	p_image->cols = cols;
	p_image->x1 = 0;
	p_image->y1 = 0;
	p_image->x2 = cols;
	p_image->y2 = lines;

	for (int k = 0; k < type; k++)
		p_image->matrix[k] = mat_aux[k];
}

//functia care rotunjeste pixelii din imagine la salvare
unsigned char round_pxl(double x)
{
	int lower = (int)x;
	int higher = lower + 1;
	if (x - lower < higher - x)
		return (unsigned char)lower;
	return (unsigned char)higher;
}

void write_image(image_t *p_image, FILE *fout, int is_binary)
{
	if (p_image->type == 1) {
		if (!is_binary)
			fprintf(fout, "P2\n");
		else
			fprintf(fout, "P5\n");
	} else {
		if (!is_binary)
			fprintf(fout, "P3\n");
		else
			fprintf(fout, "P6\n");
	}
	fprintf(fout, "%d %d\n255\n", p_image->cols, p_image->lines);
	unsigned char pixel;
	for (int i = 0; i < p_image->lines; i++) {
		for (int j = 0; j < p_image->cols; j++) {
			for (int k = 0; k < p_image->type; k++) {
				pixel = round_pxl(p_image->matrix[k][i][j]);
				if (!is_binary)
					fprintf(fout, "%hhu ", pixel);
				else
					fwrite(&pixel, sizeof(unsigned char), 1, fout);
			}
		}
		if (!is_binary)
			fprintf(fout, "\n");
	}
}

#endif
