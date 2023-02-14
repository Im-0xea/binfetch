#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include <sys/stat.h>

#include "pair.h"

#include "color.h"

const tcolor col = magenta;
const size_t max_tok = 1018;

char ascii_art[128][128];
tcolor ascii_cols[128] =
{
	yellow,
	white,
	magenta
};

typedef struct infob
{
	char name  [128];
	char header[128];
	char class [128];
	char endian[128];
	char arch  [128];
	char type  [128];
	char size  [128];
}
base;

int current_line;
int max_width;
int max_height;
int max_colors = 3;

#include "crypto.h"

static void set_color(const tcolor c)
{
	printf("\033[38;2;%d;%d;%dm", c.r, c.g, c.b);
}

static void set_blank(void)
{
	printf("\033[0m");
}

static void print_label(const char * label)
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

static void advance(void * tok, const size_t n, FILE * fp)
{
	if (!fread(tok, n, 1, fp))
	{
		printf("error reading from file\n");
		exit(1);
	}
}

static void spair_parser(char * out, const byte val, const spr * prs, const size_t size, const char * type)
{
	size_t c = 0;
	while(c < size)
	{
		if (prs[c].key == val)
		{
			strcpy(out, prs[c].str);
			break;
		}
		else if(++c == size)
		{
			sprintf(out, "unknown %s (0x%x)\n", type, val);
		}
	}
}

static void bpair_parser(char * out, const fbyte val, const bpr * prs, const size_t size, const char * type)
{
	size_t c = 0;
	while(c < size)
	{
		if (prs[c].key == val)
		{
			strcpy(out, prs[c].str);
			break;
		}
		else if(++c == size)
		{
			sprintf(out, "unknown %s (0x%x)\n", type, val);
		}
	}
}

static void address_parser(char * out, byte *tok, int bits)
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

#include"elf_parser.h"
#include"mach_parser.h"
#include"pe_parser.h"

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
	
	fbyte tok = 0;
	
	advance(&tok, 4, fp);
	
	if (tok == 0x464c457f)
	{
		strcpy(bs.header, "ELF");
		elf_parser(fp, &bs);
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
		printf("unknown %x\n", tok << 16);
	}
	
	struct stat st;
	stat(path, &st);
	size_t sz = st.st_size;
	sprintf(bs.size, "0x%lx", sz);
	
	print_label("Name");
	puts(bs.name);
	print_label("Header");
	puts(bs.header);
	print_label("Class");
	puts(bs.class);
	print_label("Endian");
	puts(bs.endian);
	print_label("Arch");
	puts(bs.arch);
	print_label("Type");
	puts(bs.type);
	print_label("Size");
	puts(bs.size);
	
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
	
	const char * short_options = "h:v";
	const struct option long_options[] =
	{
		{
			"help",    no_argument, 0, 'h'
		},
		
		{
			"version", no_argument, 0, 'V'
		},
		
		{
			0, 0, 0, 0
		},
	};
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
				printf("help\n");
				return 0;
			case 'V':
				printf("v1");
				return 0;
			default:
		}
	}
	
	if (optind >= argc)
	{
		fprintf(stderr, "Expected argument after options\n");
		exit(EXIT_FAILURE);
	}
	
	for(; optind < argc; optind++)
	{
		fetch(argv[optind]);
	}
	
	return 0;
}
