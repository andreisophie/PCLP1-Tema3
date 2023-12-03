// Copyright Andrei Maruntis 2022, 313CA

#ifndef __HUB_H__
#define __HUB_H__

#include "image.h"
#include "imageio.h"
#include "imagetools.h"
#include "hub.c"

void load_hub(image_t *p_image, char *parameters[PARAM_MAX]);

void save_hub(image_t *p_image, char *parameters[PARAM_MAX]);

int valid_number(char *parameter);

void select_hub(image_t *p_image, char *parameters[PARAM_MAX]);

void rotate_hub(image_t *p_image, char *parameters[PARAM_MAX]);

void crop_hub(image_t *p_image);

void apply_hub(image_t *p_image, char *parameters[PARAM_MAX]);

void exit_hub(image_t *p_image);

#endif
