// Copyright Andrei Maruntis 2022, 313CA

#ifndef __PARSER_C__
#define __PARSER_C__

#include "hub.h"
#include "constants.h"

// Setul de functii care editeaza imaginea incarcata in memorie
void edit_functions(image_t *p_image, char *action, char *parameters[PARAM_MAX])
{
	if (!strcmp(action, "SELECT")) {
		select_hub(p_image, parameters);
	} else {
		if (!strcmp(action, "ROTATE")) {
			rotate_hub(p_image, parameters);
		} else {
			if (!strcmp(action, "CROP")) {
				crop_hub(p_image);
			} else {
				if (!strcmp(action, "APPLY"))
					apply_hub(p_image, parameters);
				else
					printf("Invalid command\n");
			}
		}
	}
}

// functia care parseaza inputul in format
// comanda parametru1 parametru2 etc
// considera ca o comanda trebuie sa apara complet pe un singur rand
void parse_input(image_t *p_image)
{
	char input[CHAR_MAX];
	char *action;
	char *parameters[PARAM_MAX];
	int no_parameters;

	while (1) {
		for (int i = 0; i < PARAM_MAX; i++)
			parameters[i] = NULL;

		no_parameters = 0;

		fgets(input, CHAR_MAX, stdin);

		// parsarea inputului se face cu strtok
		// action si parameters[i] sunt pointeri catre char care indica catre
		// zona de memorie a vectorului input[] alocat static
		// transmit ca parametrii acesti pointeri
		action = strtok(input, "\n ");

		do {
			parameters[no_parameters++] = strtok(NULL, " ");
		} while (parameters[no_parameters - 1] && no_parameters < PARAM_MAX);
		no_parameters--;
		if (no_parameters > 0)
			parameters[no_parameters - 1]
				[strlen(parameters[no_parameters - 1]) - 1] = '\0';

		if (!strcmp(action, "EXIT")) {
			exit_hub(p_image);
			break;
		}
		if (!strcmp(action, "LOAD")) {
			load_hub(p_image, parameters);
		} else {
			if (p_image->type == 0) {
				printf("No image loaded\n");
			} else {
				if (!strcmp(action, "SAVE"))
					save_hub(p_image, parameters);
				else
					edit_functions(p_image, action, parameters);
			}
		}
	}
}

#endif
