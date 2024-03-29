#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <zlib.h>

#include "color.h"
#include "info.h"

#include "binfetch.h"

#define CHUNK_SIZE 1024

unsigned int get_checksum(FILE * fp)
{
	unsigned int sum = crc32(0L, Z_NULL, 0);
	unsigned char buffer[CHUNK_SIZE];

	size_t x;
	while ((x = fread(buffer, 1, CHUNK_SIZE, fp)) > 0)
		sum = crc32(sum, buffer, x);

	rewind(fp);

	return sum;
}

void drunken_bishop(FILE * fp)
{
	const int w = 20;
	const int h = 16;
	const int it = 100;
	const char set[] = " .o+=*B0X@%&#/^SE";

	srandom(get_checksum(fp));

	uint8_t map[h][w];
	for (int bz = 0; bz < h; ++bz)
		bzero(map[bz], w);

	int x = random() % w, y = random() % h;
	for (int i = 0; i < it; ++i) {
		switch (random() % 4) {
		case 0: // up left
			if (x > 0) x--;
			if (y < h - 1) y++;
			break;
		case 1: // up right
			if (x < w - 1) x++;
			if (y < h - 1) y++;
			break;
		case 2: // down left
			if (x > 0) x--;
			if (y > 0) y--;
			break;
		case 3: // down right
			if (x < w - 1) x++;
			if (y > 0) y--;
			break;
		}
		++map[y][x];
	}

	max_width = w;
	max_height = h;
	for (y = 0; y < h; ++y) {
		for (x = 0; x < w; ++x) {
			ascii_art[y][x] = set[MAX(0, MIN(map[y][x], 16))];
		}
	}
}
