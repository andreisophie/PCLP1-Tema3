// Copyright Andrei Maruntis 2022, 313CA

#include <stdio.h>
#include "parser.h"

// FunctiA main contine imaginea din memorie (pe care o transmit ca parametru
// tuturor functiilor) si apeleaza functia de parsare
int main(void)
{
	image_t image;

	image.type = 0;

	parse_input(&image);

	return 0;
}
