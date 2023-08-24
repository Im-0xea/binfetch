#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pair.h"

void address_parser(char * out, uint8_t *tok, int bits)
{
	strcpy(out, "0x");
	signed long b = bits == 1 ? 4 : 8;
	while (--b >= 0) {
		if (!tok[b])
			continue;
		char bit[3];
		sprintf(bit, "%02x", tok[b]);
		strcat(out, bit);
	}
}

void get_size(char * out, size_t in)
{
	const char * ends[7] = {
		"B",
		"kB",
		"MB",
		"GB",
		"TB",
		"PB",
		"EB"
	};

	int p = 1;
	while (in >= pow(2, 10 * p))
		p++;
	if (p > 7)
		p = 7;
	sprintf(out, "%.1f %s", (double) in / pow(2, 10 * (p - 1)), ends[p - 1]);
}

void advance(void * tok, const size_t n, FILE * fp)
{
	if (!fread(tok, n, 1, fp)) {
		printf("error reading from file\n");
		exit(1);
	}
}
