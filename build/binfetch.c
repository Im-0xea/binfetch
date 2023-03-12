#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include <sys/stat.h>

#include "info.h"
#include "pair.h"
#include "config.h"

#include "color.h"
#include "bin_op.h"

char ascii_art  [128][128];
char ibuffer[128][128];
int  buffer_pos = 0;
tcolor ascii_cols[128];
tcolor main_cols[128];

int   label_count;
label labels[128];

int  label_order_count = 0;
char label_order[128][128];

int current_line;
int max_width;
int max_height;
int flag_max_colors = 0;
int main_max_colors = 0;

extern void elf_parser(FILE * fp);
extern void mach_parser(FILE * fp, int bit, int end);
extern void pe_parser(FILE * fp);
extern void mz_parser(FILE * fp);
extern void sh_parser(FILE * fp);
extern void checksum_art(FILE * fp);

void print_label(const char * label)
{
	if (ascii_art[current_line][0] != '\0')
	{
		set_color(ascii_cols[(int) ((float) current_line / (float) (max_height) * (float) flag_max_colors)]);
		printf("%s  ", ascii_art[current_line++]);
		set_blank();
	}
	else if (current_line != 0)
	{
		printf("%*s  ", max_width, " ");
	}
	set_color(main_cols[(int) ((float) current_line / (float) (label_count + 1) * (float) main_max_colors)]);
	printf("%s: ", label);
	set_blank();
}

int fetch(char * path)
{
	
	FILE * fp = fopen(path, "rb");
	
	if (!fp)
	{
		set_color(red);
		printf("failed to open %s\n", path);
		set_blank();
		return 1;
	}
	
	label_count = 0;
	current_line = 0;
	max_width = 0;
	max_height = 0;
	
	for (int i = 0; i < 64; i++)
	{
		bzero(ascii_art[i], 64);
	}
	
	drunken_bishop(fp);
	
	strcpy(ibuffer[buffer_pos], basename(path));
	add_label("Name", ibuffer[buffer_pos++]);
	
	fbyte tok = 0;
	
	advance(&tok, 4, fp);
	
	if (tok == 0x464c457f)
	{
		strcpy(ibuffer[buffer_pos], "ELF\0");
		add_label("Header", ibuffer[buffer_pos++]);
		elf_parser(fp);
	}
	else if (tok == 0xfeedfacf || tok == 0xfeedfacf || tok == 0xcefaedfe || tok == 0xcffaedfe)
	{
		strcpy(ibuffer[buffer_pos], "MACH-O");
		add_label("Header", ibuffer[buffer_pos++]);
		int cond = tok >> 24 != 0xfe;
		mach_parser(fp, cond, !cond ? tok << 24 == 0xce ? 0 : 1 : tok >> 24 == 0xce ? 0 : 1);
	}
	else if (tok == 0xfa405a4d)
	{
		strcpy(ibuffer[buffer_pos], "COMPRESSED BIN");
		add_label("Header", ibuffer[buffer_pos++]);
	}
	else if ((tok & 0x0000ffff) == 0x5a4d)
	{
		advance(&tok, 0xd8 - 4, fp);
		advance(&tok, 4, fp);
		if (tok != 0x5045)
		{
			rewind(fp);
			strcpy(ibuffer[buffer_pos], "MZ");
			add_label("Header", ibuffer[buffer_pos++]);
			mz_parser(fp);
		}
		else
		{
			strcpy(ibuffer[buffer_pos], "PE");
			add_label("Header", ibuffer[buffer_pos++]);
			pe_parser(fp);
		}
	}
	else if (tok == 0x0a324655)
	{
		strcpy(ibuffer[buffer_pos], "UF2");
		add_label("Header", ibuffer[buffer_pos++]);
	}
	else if ((tok & 0xffff) == 0x2123)
	{
		strcpy(ibuffer[buffer_pos], "Script");
		add_label("Header", ibuffer[buffer_pos++]);
		sh_parser(fp);
	}
	else if (tok == 0xbebafeca)
	{
		strcpy(ibuffer[buffer_pos], "Java Class");
		add_label("Header", ibuffer[buffer_pos++]);
	}
	else
	{
		sprintf(ibuffer[buffer_pos], "unknown 0x%x", tok);
		add_label("Header", ibuffer[buffer_pos++]);
	}
	
	struct stat st;
	stat(path, &st);
	get_size(ibuffer[buffer_pos], st.st_size);
	add_label("Size", ibuffer[buffer_pos++]);
	
	for (int x = 0; x < label_order_count; x++)
	{
		for (int y = 0; y < label_count; y++)
		{
			if (!strcmp(label_order[x], labels[y].key))
			{
				print_label(labels[y].key);
				puts(labels[y].out);
				labels[y].used = true;
			}
		}
	}
	
	for (int z = 0; z < label_count; z++)
	{
		if (!labels[z].used)
		{
			print_label(labels[z].key);
			puts(labels[z].out);
		}
	}
	
	for (; current_line < 64; current_line++)
	{
		if (ascii_art[current_line][0] != '\0')
		{
			set_color(ascii_cols[(int) ((float) (current_line) / (float) max_height * (float) flag_max_colors)]);
			printf("%s\n", ascii_art[current_line]);
			set_blank();
		}
	}
	printf("\n");
	
	return fclose(fp);
}
