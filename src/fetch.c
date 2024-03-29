#include <arpa/inet.h>
#include <errno.h>
#include <libgen.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bin_op.h"
#include "color.h"
#include "config.h"
#include "crypto.h"
#include "elf_parser.h"
#include "info.h"
#include "jvm.h"
#include "mach.h"
#include "mz.h"
#include "pair.h"
#include "pe.h"
#include "sh.h"
#include "uf2.h"

char ascii_art[128][128];

struct rgb_color ascii_cols[128];
struct rgb_color main_cols[128];

char ibuffer[128][128];
int  buffer_pos = 0;

int label_count;
struct label labels[128];

int  label_order_count = 0;
char label_order[128][128];

bool bold = false;

int current_line;
int max_width;
int max_height;
int flag_max_colors = 0;
int main_max_colors = 0;

void print_label(const char * label, const bool name)
{
	const char set[] = " .o+=*B0X@%&#/^SE";
	if (ascii_art[current_line][0] != '\0') {
		int x = 0;
		int ckdi = 0;
		while (ascii_art[current_line][x] != '\0') {
			if (bold)
				set_bold();
			switch (cclr) {
			case hor:
				set_color(ascii_cols[(int)floor((float) current_line / (float) (max_height) * (float) flag_max_colors)]);
				break;
			case ver:
				set_color(ascii_cols[(int)floor((float) x / (float) (max_width) * (float) flag_max_colors)]);
				break;
			case dia:
				set_color(ascii_cols[(int)floor((float) (current_line + x) / (float) (max_width + max_height) * (float) flag_max_colors)]);
				break;
			case val:
				ckdi = 0;
				while (ckdi < 14) {
					if (ascii_art[current_line][x] == set[ckdi])
						break;
					++ckdi;
				}
				set_color(ascii_cols[(int)floor((float) ckdi / (float) 14 * (float) flag_max_colors)]);
				break;
			}
			printf("%c", ascii_art[current_line][x]);
			set_blank();
			x++;
		}
		printf("  ");
	} else if (current_line != 0) {
		printf("%*s  ", max_width, " ");
	}
	if (bold)
		set_bold();
	set_color(main_cols[(int)floor((float) current_line / (float) (label_count + 2) * (float) main_max_colors)]);
	if (!name)
		printf("%s: ", label);
	set_blank();
	current_line++;
}

void fetch(char * path)
{
	FILE * fp = fopen(path, "rb");

	if (!fp) {
		fprintf(stderr, "failed to open binary %s: %s\n", path, strerror(errno));
		exit(1);
	}

	label_count = 0;
	current_line = 0;
	max_width = 0;
	max_height = 0;

	for (int i = 0; i < 64; i++) bzero(ascii_art[i], 64);

	drunken_bishop(fp);

	uint32_t tok = 0;

	advance(&tok, 4, fp);

	if (tok == 0x464c457f) {
		strcpy(ibuffer[buffer_pos], "ELF\0");
		add_label("Header", ibuffer[buffer_pos++]);

		elf_parser(fp);
	} else if (tok == 0xfeedfacf || tok == 0xcefaedfe || tok == 0xcffaedfe) {
		strcpy(ibuffer[buffer_pos], "MACH-O");
		add_label("Header", ibuffer[buffer_pos++]);

		mach_parser(fp, tok);
	} else if (tok == 0xbebafeca) {
		advance(&tok, 4, fp);
		tok = ntohl(tok);
		if (tok > 0x20) {
			strcpy(ibuffer[buffer_pos], "Java Class");
			add_label("Header", ibuffer[buffer_pos++]);

			jvm_parser(fp, tok);
		} else {
			strcpy(ibuffer[buffer_pos], "Universal Binary");
			add_label("Header", ibuffer[buffer_pos++]);

			mach_universal_parser(fp);
		}
	} else if (tok == 0xfa405a4d) {
		strcpy(ibuffer[buffer_pos], "COMPRESSED BIN");
		add_label("Header", ibuffer[buffer_pos++]);
	} else if ((tok & 0x0000ffff) == 0x5a4d) {
		fseek(fp, 0xd8 - 4, SEEK_SET);
		advance(&tok, 4, fp);
		if (tok != 0x5045) {
			rewind(fp);
			strcpy(ibuffer[buffer_pos], "MZ");
			add_label("Header", ibuffer[buffer_pos++]);

			mz_parser(fp);
		} else {
			strcpy(ibuffer[buffer_pos], "PE");
			add_label("Header", ibuffer[buffer_pos++]);

			pe_parser(fp);
		}
	} else if (tok == 0x0a324655) {
		strcpy(ibuffer[buffer_pos], "UF2");
		add_label("Header", ibuffer[buffer_pos++]);

		uf2_parser(fp);
	} else if ((tok & 0xffff) == 0x2123) {
		strcpy(ibuffer[buffer_pos], "Script");
		add_label("Header", ibuffer[buffer_pos++]);

		sh_parser(fp);
	} else {
		sprintf(ibuffer[buffer_pos], "unknown 0x%x", tok);
		add_label("Header", ibuffer[buffer_pos++]);
	}

	fseek(fp, 0L, SEEK_END);
	size_t sz = ftell(fp);
	fclose(fp);
	
	get_size(ibuffer[buffer_pos], sz);
	add_label("Size", ibuffer[buffer_pos++]);

	print_label("Name", true);
	puts(basename(path));
	print_label("Name", true);
	for (int dl = 0; dl < strlen(basename(path)); dl++)
		printf("%c", '-');
	printf("%c", '\n');

	for (int x = 0; x < label_order_count; x++) {
		for (int y = 0; y < label_count; y++) {
			if (!strcmp(label_order[x], labels[y].key)) {
				print_label(labels[y].key, false);
				puts(labels[y].out);
				labels[y].used = true;
			}
		}
	}

	for (int z = 0; z < label_count; z++) {
		if (!labels[z].used) {
			print_label(labels[z].key, false);
			puts(labels[z].out);
		}
	}

	for (; current_line < max_height;) {
		if (ascii_art[current_line][0] != '\0') {
			print_label("end", true);
			printf("%c", '\n');
		}
	}

	printf("\n");
}
