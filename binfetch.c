#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include <sys/stat.h>

#include <openssl/sha.h>


#include "elf_arch.h"
#include "elf_type.h"
#include "elf_osabi.h"

#include "mach_arch.h"
#include "mach_type.h"


typedef enum ansi_color
{
	blank   = 0,
	red     = 31,
	green   = 32,
	yellow  = 33,
	blue    = 34,
	magenta = 35,
	cyan    = 36,
	white   = 37
}
color;


const color col = magenta;
const size_t max_tok = 128;

char ascii_art[64][64];
int current_line = 0;
int max_line = 0;


#include "crypto.h"


static void set_color(const color c)
{
	printf("\033[%dm", c);
}

static void print_label(const char * label)
{
	if (ascii_art[current_line][0] != '\0')
	{
		printf("%s  ", ascii_art[current_line++]);
	}
	else if (current_line != 0)
	{
		printf("%*s  ", max_line, " ");
	}
	set_color(col);
	printf("%s: ", label);
	set_color(blank);
}

static void advance(void * tok, const size_t n, FILE * fp)
{
	bzero(tok, max_tok);
	if (!fread(tok, n, 1, fp))
	{
		printf("error reading from file\n");
		exit(1);
	}
}

static void spair_parser(const byte val, const spr * prs, const size_t size, const char * type)
{
	size_t c = 0;
	while(c < size)
	{
		if (prs[c].key == val)
		{
			printf("%s\n", prs[c].str);
			break;
		}
		else if(++c == size)
		{
			printf("unknown %s (0x%x)\n", type, val);
		}
	}
}

static void bpair_parser(const fbyte val, const bpr * prs, const size_t size, const char * type)
{
	size_t c = 0;
	while(c < size)
	{
		if (prs[c].key == val)
		{
			printf("%s\n", prs[c].str);
			break;
		}
		else if(++c == size)
		{
			printf("unknown %s (0x%x)\n", type, val);
		}
	}
}

#include"elf_parser.h"
#include"mach_parser.h"
#include"pe_parser.h"

int main(int argc, char **argv)
{
	
	if (argc < 2)
	{
		set_color(red);
		printf("you did not provide a binary\n");
		set_color(blank);
		return 1;
	}
	
	FILE * fp = fopen(argv[1], "rb");
	
	for (int i = 0; i < 64; i++)
	{
		bzero(ascii_art[i], 64);
	}
	sha512_art(fp);
	
	if (!fp)
	{
		set_color(red);
		printf("failed to open binary\n");
		set_color(blank);
		return 1;
	}
	
	print_label("Name");
	
	puts(basename(argv[1]));
	
	
	fbyte tok = 0;
	
	advance(&tok, 4, fp);
	
	print_label("Header");
	
	if (tok == 0x464c457f)
	{
		printf("ELF\n");
		elf_parser(fp);
	}
	else if (tok == 0xfeedfacf || tok == 0xfeedfacf || tok == 0xcefaedfe || tok == 0xcffaedfe)
	{
		printf("MACH-O\n");
		int cond = tok >> 24 != 0xfe;
		mach_parser(fp, cond, !cond ? tok << 24 == 0xce ? 0 : 1 : tok >> 24 == 0xce ? 0 : 1);
	}
	else if (tok == 0xfa405a4d)
	{
		printf("Compressed Binary\n");
	}
	else if ((tok & 0x0000ffff) == 0x5a4d)
	{
		printf("PE\n");
		pe_parser(fp);
	}
	else if (tok == 0x0a324655)
	{
		printf("uf2\n");
	}
	else
	{
		printf("unknown %x\n", tok << 16);
	}
	print_label("Size");
	
	struct stat st;
	stat(argv[1], &st);
	size_t sz = st.st_size;
	printf("%lu\n", sz);
	
	for (; current_line < 64; current_line++)
	{
		if (ascii_art[current_line][0] != '\0')
		{
			printf("%s\n", ascii_art[current_line]);
		}
	}
	
	printf("\n");
	
	fclose(fp);
	return 0;
}
