#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "info.h"
#include "pair.h"

void spair_parser(char * out, const uint8_t val, const struct spr * prs, const size_t size, const char * type)
{
	size_t c = 0;
	while(c < size) {
		if (prs[c].key == val) {
			strcpy(out, prs[c].str);
			break;
		} else if(++c == size) {
			sprintf(out, "unknown %s (0x%x)\n", type, val);
		}
	}
}

void bpair_parser(char * out, const uint32_t val, const struct bpr * prs, const size_t size, const char * type)
{
	size_t c = 0;
	while(c < size) {
		if (prs[c].key == val) {
			strcpy(out, prs[c].str);
			break;
		} else if(++c == size) {
			sprintf(out, "unknown %s (0x%x)\n", type, val);
		}
	}
}

