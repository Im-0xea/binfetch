#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "pair.h"
#include "info.h"

#include "binfetch.h"

#ifndef PREFIX
	#define PREFIX "/tmp"
#endif

enum coloring cclr = hor;

void parse_cfg(const char * defined_path)
{
	FILE * fp;
	if (!defined_path) {
		char path[128];
		strcpy(path, getenv("HOME"));
		strcat(path, "/.config/binfetch/binfetch.cfg");
		if (!(fp = fopen(path, "r"))) {
			strcpy(path, PREFIX);
			strcat(path, "/share/binfetch/binfetch.cfg");
			if (!(fp = fopen(path, "r"))) {
				printf("failed to open standard config\n");
				exit(1);
			}
		}
	} else {
		fp = fopen(defined_path, "r");
		if (!fp) {
			printf("failed to open defined config\n");
			exit(1);
		}
	}

	char tok[128];
	while (fgets(tok, 128, fp)) {
		if (!strncmp("[art_colors]", tok, 12)) {
			while (fgets(tok, 128, fp) && tok[0] == '\t') {
				sscanf(tok + 1, "#%2x%2x%2x", (unsigned int *) &ascii_cols[flag_max_colors].r, (unsigned int *) &ascii_cols[flag_max_colors].g, (unsigned int *) &ascii_cols[flag_max_colors].b);
				++flag_max_colors;
			}
		}
		if (!strncmp("[label_colors]", tok, 14)) {
			while (fgets(tok, 128, fp) && tok[0] == '\t') {
				sscanf(tok + 1, "#%2x%2x%2x", (unsigned int *) &main_cols[main_max_colors].r, (unsigned int *) &main_cols[main_max_colors].g, (unsigned int *) &main_cols[main_max_colors].b);
				++main_max_colors;
			}
		}
		if (!strncmp("[order]", tok, 7)) {
			while (fgets(tok, 128, fp) && tok[0] == '\t') {
				tok[strlen(tok) - 1] = '\0';
				strcpy(label_order[label_order_count++], tok + 1) ;
			}
		}

		if (!strncmp("[pattern]", tok, 9)) {
			while (fgets(tok, 128, fp) && tok[0] == '\t') {
				if (!strncmp("horizontal", tok + 1, 10)) {
					cclr = hor;
					break;
				}
				if (!strncmp("vertical", tok + 1, 8)) {
					cclr = ver;
					break;
				}
				if (!strncmp("diagonal", tok + 1, 8)) {
					cclr = dia;
					break;
				}
				if (!strncmp("value", tok + 1, 5)) {
					cclr = val;
					break;
				}
			}
		}

		if (!strncmp("[attributes]", tok, 7)) {
			while (fgets(tok, 128, fp) && tok[0] == '\t') {
				if (!strncmp("bold", tok + 1, 4)) {
					bold = true;
					break;
				}
			}
		}
	}

	fclose(fp);
}
