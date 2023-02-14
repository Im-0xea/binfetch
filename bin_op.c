#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pair.h"

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

void advance(void * tok, const size_t n, FILE * fp)
{
	if (!fread(tok, n, 1, fp))
	{
		printf("error reading from file\n");
		exit(1);
	}
}
