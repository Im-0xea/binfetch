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
	if (ascii_art[current_line][0] != NULL)
	{
		printf("%s  ", ascii_art[current_line++]);
	}
	else
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
			printf("unknown %s (%d)\n", type, val);
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
			printf("unknown %s (0x%lx)\n", type, val);
		}
	}
}

static void elf_parser(FILE * fp)
{
	byte tok[max_tok];
	
	advance(tok, 5, fp);
	
	int bits = 0 ;// 32 or 64
	
	print_label("Class");
	
	if (tok[0] == 0x01)	
	{
		bits = 1;
		printf("32 bit\n");
	}
	else if (tok[0] == 0x02)
	{
		bits = 2;
		printf("64 bit\n");
	}
	else
	{
		printf("unknown");
	}
	
	print_label("Endianness");
	
	if (tok[1] == 0x01)
	{
		printf("little endian\n");
	}
	else if (tok[1] == 0x02)
	{
		printf("big endian\n");
	}
	else
	{
		printf("unknown endian\n");
	}
	
	print_label("Version");
	
	if (tok[2] == 0x01)
	{
		printf("ELFv1\n");
	}
	else if (tok[2] == 0x00)
	{
		printf("noncompliant\n");
	}
	
	print_label("OS ABI");
	
	spair_parser(tok[3], osabis, sizeof osabis / sizeof(spr), "abi");
	
	if (tok[4] != 0)
	{
		print_label("ABI Version");
		
		printf("%d\n", tok[4]);
	}
	
	while (fread(tok, 1, 1, fp) && tok[0] == 0);
	fseek(fp, -1, SEEK_CUR);
	// do nothing
	
	advance(tok, 2, fp);
	
	print_label("Type");
	
	spair_parser(tok[0], elf_types, sizeof elf_types / sizeof(spr), "type");
	
	advance(tok, 2, fp);
	
	print_label("Arch");
	
	spair_parser(tok[0], elf_arches, sizeof elf_arches / sizeof(spr), "arch");
	
	advance(tok, 4, fp);
	
	// do nothing
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	print_label("Entry");
	
	printf("0x");
	
	{
		char out[128] = "";
		signed long b = bits == 1 ? 4 : 8;
		while (--b >= 0)
		{
			if (!tok[b]) continue;
			char bit[3];
			sprintf(bit, "%02x", tok[b]);
			strcat(out,bit);
		}
		
		printf("%s\n", out);
	}
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	print_label("Table");
	printf("0x");
	
	{
		char out[128] = "";
		signed long b = bits == 1 ? 4 : 8;
		while (--b >= 0)
		{
			if (!tok[b]) continue;
			char bit[3];
			sprintf(bit, "%02x", tok[b]);
			strcat(out,bit);
		}
		
		printf("%s\n", out);
	}
}

static void mach_parser(FILE * fp, int bit, int end)
{
	fbyte tok = 0;
	int bits = 0 ;// 32 or 64
	
	advance(&tok, 4, fp);
	
	print_label("Arch");
	
	bpair_parser(tok, mach_arches, sizeof mach_arches / sizeof(bpr), "cpu type");
	
	fbyte cls = (tok >> 24);
	print_label("Class");
	
	if (cls == 0x00)	
	{
		bits = 1;
		printf("32 bit\n");
	}
	else if (cls == 0x1)
	{
		bits = 2;
		printf("64 bit\n");
	}
	else if (cls == 0x2)
	{
		bits = 2;
		printf("LP 32\n");
	}
	else
	{
		printf("unknown (%lx)", cls);
	}
	
	print_label("Endianness");
	if (end == 0)
	{
		printf("little endian\n");
	}
	else if (end == 1)
	{
		printf("big endian\n");
	}
	else
	{
		printf("unknown endian\n");
	}
	
	advance(&tok, 4, fp);
	advance(&tok, 4, fp);
	
	print_label("Type");
	
	spair_parser(tok, mach_types, sizeof mach_types / sizeof(spr), "type");
	
	
	return;
}

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
	else if (tok == 0x0a324655)
	{
		printf("uf2\n");
	}
	else
	{
		printf("unknown %lx\n", tok);
	}
	print_label("Size");
	
	struct stat st;
	stat(argv[1], &st);
	size_t sz = st.st_size;
	printf("%lu\n", sz);
	
	for (; current_line < 64; current_line++)
	{
		if (ascii_art[current_line][0] != NULL)
		{
			printf("%s\n", ascii_art[current_line]);
		}
	}
	
	printf("\n");
	
	fclose(fp);
	return 0;
}
