#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include <sys/stat.h>

#include "info.h"
#include "pair.h"

#include "color.h"

const tcolor col = magenta;

char ascii_art[128][128];
tcolor ascii_cols[128] =
{
	yellow,
	white,
	magenta
};

int   label_count;
label labels[128];

int  label_order_count = 0;
char label_order[128][128];

int current_line;
int max_width;
int max_height;
int max_colors = 0;

#include "crypto.h"
#include "config.h"

void set_color(const tcolor c)
{
	printf("\033[38;2;%d;%d;%dm", c.r, c.g, c.b);
}

void set_blank(void)
{
	printf("\033[0m");
}

void print_label(const char * label)
{
	if (ascii_art[current_line][0] != '\0')
	{
		set_color(ascii_cols[(int) ((float) current_line / (float) max_height * (float) max_colors)]);
		printf("%s  ", ascii_art[current_line++]);
		set_blank();
	}
	else if (current_line != 0)
	{
		printf("%*s  ", max_width, " ");
	}
	set_color(col);
	printf("%s: ", label);
	set_blank();
}

void advance(void * tok, const size_t n, FILE * fp)
{
	if (!fread(tok, n, 1, fp))
	{
		printf("error reading from file\n");
		exit(1);
	}
}

void address_parser(char * out, byte *tok, int bits)
{
	strcpy(out, "0x");
	signed long b = bits == 1 ? 4 : 8;
	while (--b >= 0)
	{
		if (!tok[b]) continue;
		char bit[3];
		sprintf(bit, "%02x", tok[b]);
		strcat(out, bit);
	}
}

void add_label(char * name, char * out)
{
	strcpy(labels[label_count].key, name);
	labels[label_count].out = out;
	labels[label_count++].used = false;
}

void get_size(char * out, size_t in)
{
	if (in < 1024)
	{
		sprintf(out, "%ld b", in);
	}
	else if (in < 1048576)
	{
		sprintf(out, "%.1f kb", (double) in / 1024);
	}
	else if (in < 1073741824)
	{
		sprintf(out, "%.1f mb", (double) in / 1048576);
	}
	else if (in < 1099511627776)
	{
		sprintf(out, "%.1f gb", (double) in / 1073741824);
	}
	else if (in < 1125899906842624)
	{
		sprintf(out, "%.1f tb", (double) in / 1099511627776);
	}
	else if (in < 1152921504606846976)
	{
		sprintf(out, "%.1f pb", (double) in / 1125899906842624);
	}
	else
	{
		sprintf(out, "%.1f eb", (double) in / 1152921504606846976);
	}
}

extern void elf_parser(FILE * fp, base * bs, elf * as);
extern void mach_parser(FILE * fp, base * bs, int bit, int end);
extern void pe_parser(FILE * fp, base * bs);

static int fetch(char * path)
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
	
	checksum_art(fp, EVP_sha512());
	
	base bs;
	
	strcpy(bs.name, basename(path));
	add_label("Name", bs.name);
	
	fbyte tok = 0;
	
	advance(&tok, 4, fp);
	
	elf as;
	if (tok == 0x464c457f)
	{
		strcpy(bs.header, "ELF\0");
		elf_parser(fp, &bs, &as);
	}
	else if (tok == 0xfeedfacf || tok == 0xfeedfacf || tok == 0xcefaedfe || tok == 0xcffaedfe)
	{
		strcpy(bs.header, "MACH-O");
		int cond = tok >> 24 != 0xfe;
		mach_parser(fp,&bs, cond, !cond ? tok << 24 == 0xce ? 0 : 1 : tok >> 24 == 0xce ? 0 : 1);
	}
	else if (tok == 0xfa405a4d)
	{
		strcpy(bs.header, "COMPRESSED BIN");
	}
	else if ((tok & 0x0000ffff) == 0x5a4d)
	{
		strcpy(bs.header, "PE");
		pe_parser(fp, &bs);
	}
	else if (tok == 0x0a324655)
	{
		strcpy(bs.header, "UF2");
	}
	else
	{
		sprintf(bs.header, "unknown %x\n", tok << 16);
	}
	add_label("Header", bs.header);
	
	struct stat st;
	stat(path, &st);
	get_size(bs.size, st.st_size);
	add_label("Size", bs.size);
	
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
			set_color(ascii_cols[(int) ((float) (current_line) / (float) max_height * (float) max_colors)]);
			printf("%s\n", ascii_art[current_line]);
			set_blank();
		}
	}
	
	return fclose(fp);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		set_color(red);
		printf("you did not provide a binary\n");
		set_blank();
		return 1;
	}
	
	const char * short_options = "h:v:c";
	const struct option long_options[] =
	{
		{
			"help",    no_argument,       0, 'h'
		},
		
		{
			"version", no_argument,       0, 'V'
		},
		
		{
			"config",  required_argument, 0, 'c'
		},
		
		{
			0, 0, 0, 0
		},
	};
	
	char config[128] = "";
	while (1)
	{
		const int opt = getopt_long(argc, argv, short_options, long_options, NULL);
		
		if (-1 == opt)
		{
			break;
		}
		
		switch (opt)
		{
			case '?': ;// Unrecognized option
			case 'h':
				printf("Usage: binfetch [OPTION] [EXECUTABLES]\n\n" \
				     " -h, --help    -> prints this\n" \
				     " -V, --version -> prints version\n" \
				     " -c, --config  -> set config file\n");
				return 0;
			case 'V':
				printf("v1");
				return 0;
			case 'c':
				strcpy(config, optarg);
				break;
			default:
		}
	}
	
	if (optind >= argc)
	{
		fprintf(stderr, "Expected argument after options\n");
		exit(EXIT_FAILURE);
	}
	
	if (parse_cfg(config))
	{
		return 1;
	}
	
	
	for(; optind < argc; optind++)
	{
		fetch(argv[optind]);
	}
	
	return 0;
}
