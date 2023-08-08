#pragma once

struct rgb_color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

static const struct rgb_color white = {
	255,
	255,
	255
};

static const struct rgb_color cyan = {
	109,
	209,
	255
};

static const struct rgb_color magenta = {
	156,
	89,
	209
};

static const struct rgb_color light_magenta = {
	234,
	150,
	170
};

static const struct rgb_color red = {
	255,
	0,
	0
};

static const struct rgb_color grey = {
	170,
	170,
	170
};

static const struct rgb_color yellow = {
	252,
	244,
	52
};

enum coloring {
	hor,
	ver,
	dia,
	val
};

extern enum coloring cclr;

void set_color(const struct rgb_color c);

static inline void set_bold(void)
{
	printf("\033[1m");
}
static inline void set_blank(void)
{
	printf("\033[0m");
}
