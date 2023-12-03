// Copyright Andrei Maruntis 2022, 313CA

#ifndef __PARSER_H__
#define __PARSER_H__

#include "hub.h"
#include "parser.c"

void edit_functions(image_t *p_image, char *action,
					char *parameters[PARAM_MAX]);

void parse_input(image_t *p_image);

#endif
