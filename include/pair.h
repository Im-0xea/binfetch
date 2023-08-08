#pragma once

struct spr {
	char key;
	char str[64];
};

struct bpr {
	uint32_t key;
	char str[64];
};

extern void spair_parser(char * out, const uint8_t val, const struct spr * prs, const size_t size, const char * type);
extern void bpair_parser(char * out, const uint32_t val, const struct bpr * prs, const size_t size, const char * type);
