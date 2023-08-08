#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "color.h"
#include "pair.h"
#include "info.h"

const int ansi[16][4] = {
	{0, 0, 0, 30},
	{128, 0, 0, 31},
	{0, 128, 0, 32},
	{128, 128, 0, 33},
	{0, 0, 128, 34},
	{200, 120, 150, 35},
	{0, 128, 128, 36},
	{255, 255, 255, 37}
};

int find_closest_color(const struct rgb_color c)
{
	int min_distance = 3 * 255 * 255;
	int closest_color = -1;
	for (int i = 0; i < 16; i++) {
		const int dr = c.r - ansi[i][0];
		const int dg = c.g - ansi[i][1];
		const int db = c.b - ansi[i][2];
		const int distance = dr * dr + dg * dg + db * db;
		if (distance < min_distance) {
			min_distance = distance;
			closest_color = i;
		}
	}
	return ansi[closest_color][3];
}

void set_color(const struct rgb_color c)
{
	if (isatty(fileno(stdout))) {
		if (getenv("COLORTERM")) {
			printf("\033[38;2;%d;%d;%dm", c.r, c.g, c.b);
			return;
		} else if (getenv("TERM") && strstr(getenv("TERM"), "256")) {
			const int code = 16 + (36 * (c.r / 255.0 * 5)) + (6 * (c.g / 255.0 * 5)) + (c.b / 255.0 * 5);
			printf("\033[38;5;%dm", code);
		} else {
			printf("\033[%dm", find_closest_color(c));
		}
	}
}
