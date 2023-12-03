// Copyright Andrei Maruntis 2022, 313CA

#ifndef __HUB_C__
#define __HUB_C__

#include <string.h>
#include <ctype.h>
#include "image.h"
#include "imageio.h"
#include "imagetools.h"
#include "constants.h"

// header-ul hub contine cate o functie pentru fiecare comanda care este
// din header-ul parser; in general functiile din hub verifica validitatea
// parametrilor primiti de la parser si afiseaza mesajele de succes sau de
// eroare catre consola
// in cazul functiilor read si write ele si deschid/inchid fisierele
void load_hub(image_t *p_image, char *parameters[PARAM_MAX])
{
	free_image(p_image);

	FILE *fin = fopen(parameters[0], "rb");
	if (!fin) {
		printf("Failed to load %s\n", parameters[0]);
		return;
	}

	read_image(p_image, fin);

	if (p_image->type == 0) {
		printf("Failed to load %s\n", parameters[0]);
		return;
	}

	printf("Loaded %s\n", parameters[0]);

	fclose(fin);
}

void save_hub(image_t *p_image, char *parameters[PARAM_MAX])
{
	FILE *fout = fopen(parameters[0], "wb");
	if (!fout) {
		printf("Failed to save to %s\n", parameters[0]);
		return;
	}

	int is_binary;
	if (parameters[1] && !strcmp(parameters[1], "ascii"))
		is_binary = 0;
	else
		is_binary = 1;

	if (p_image->type != 0) {
		write_image(p_image, fout, is_binary);
		printf("Saved %s\n", parameters[0]);
	} else {
		if (p_image->type != 0) {
			write_image(p_image, fout, is_binary);
			printf("Saved %s\n", parameters[0]);
		}
	}

	fclose(fout);
}

// functie care verifica daca un string primit ca parametru este numeric
// Se foloseste la comanda select pentru a putea aplica functia atoi()
int valid_number(char *parameter)
{
	if (!parameter)
		return -1;
	int length = strlen(parameter);
	int start = 0;
	if (parameter[0] == '-' || parameter[0] == '+')
		start++;
	for (int i = start; i < length; i++) {
		if (!isdigit(parameter[i]))
			return -1;
	}
	return 0;
}

void select_hub(image_t *p_image, char *parameters[PARAM_MAX])
{
	if (!strcmp(parameters[0], "ALL")) {
		p_image->x1 = 0;
		p_image->y1 = 0;
		p_image->x2 = p_image->cols;
		p_image->y2 = p_image->lines;
		printf("Selected ALL\n");
	} else {
		for (int i = 0; i < 4; i++) {
			if (valid_number(parameters[i])) {
				printf("Invalid command\n");
				return;
			}
		}
		int x1, x2, y1, y2;
		x1 = atoi(parameters[0]);
		y1 = atoi(parameters[1]);
		x2 = atoi(parameters[2]);
		y2 = atoi(parameters[3]);

		int result = select_image(p_image, &x1, &y1, &x2, &y2);

		switch (result) {
		case (0):
			printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
			break;
		case (-1):
			printf("Invalid set of coordinates\n");
			break;
		default:
			printf("Select failed\n");
		}
	}
}

void rotate_hub(image_t *p_image, char *parameters[PARAM_MAX])
{
	int angle = atoi(parameters[0]);
	int result;

	if (angle % RIGHT_ANGLE || angle < -MAX_ANGLE || angle > MAX_ANGLE) {
		printf("Unsupported rotation angle\n");
		return;
	}

	int nr_rotations = angle / RIGHT_ANGLE;

	// daca numarul de rotatii e multiplu de patru nu fac nimic dar afisez
	// un mesaj de succes daca selectia e valida (patrata sau intreaga imagine)
	if (nr_rotations % 4 == 0) {
		if (p_image->x2 - p_image->x1 != p_image->y2 - p_image->y1 &&
			!entire_image(p_image)) {
			printf("The selection must be square\n");
			return;
		}
		printf("Rotated %d\n", angle);
		return;
	}

	if (nr_rotations < 0)
		nr_rotations += 4;

	for (int i = 0; i < nr_rotations; i++)
		result = rotate_image(p_image);

	switch (result) {
	case(0):
		printf("Rotated %d\n", angle);
		break;
	case (1):
		printf("The selection must be square\n");
		break;
	default:
		printf("Rotate failed\n");
		break;
	}
}

void crop_hub(image_t *p_image)
{
	int result = crop_image(p_image);
	switch (result) {
	case(0):
		printf("Image cropped\n");
		break;
	default:
		printf("Crop failed\n");
		break;
	}
}

void apply_hub(image_t *p_image, char *parameters[PARAM_MAX])
{
	const double filters[FILTERS][KERNEL_SIZE][KERNEL_SIZE] = {
		{
			{-1, -1, -1},
			{-1, 8, -1},
			{-1, -1, -1}
		},
		{
			{0, -1, 0},
			{-1, 5, -1},
			{0, -1, 0}
		},
		{
			{1. / 9, 1. / 9, 1. / 9},
			{1. / 9, 1. / 9, 1. / 9},
			{1. / 9, 1. / 9, 1. / 9}
		},
		{
			{1. / 16, 2. / 16, 1. / 16},
			{2. / 16, 4. / 16, 2. / 16},
			{1. / 16, 2. / 16, 1. / 16}
		}
	};

	if (!parameters[0]) {
		printf("Invalid command\n");
		return;
	}

	int result;
	if (!strcmp(parameters[0], "EDGE")) {
		result = apply_filter(p_image, filters[0]);
	} else {
		if (!strcmp(parameters[0], "SHARPEN")) {
			result = apply_filter(p_image, filters[1]);
		} else {
			if (!strcmp(parameters[0], "BLUR")) {
				result = apply_filter(p_image, filters[2]);
			} else {
				if (!strcmp(parameters[0], "GAUSSIAN_BLUR")) {
					result = apply_filter(p_image, filters[3]);
				} else {
					printf("APPLY parameter invalid\n");
					return;
				}
			}
		}
	}

	switch (result) {
	case (0):
		printf("APPLY %s done\n", parameters[0]);
		break;
	case(1):
		printf("Easy, Charlie Chaplin\n");
		break;
	default:
		printf("Apply failed\n");
		break;
	}
}

void exit_hub(image_t *p_image)
{
	if (p_image->type != 0)
		free_image(p_image);
	else
		printf("No image loaded\n");
}

#endif
