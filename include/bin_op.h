#pragma once

extern void address_parser(char * out, uint8_t *tok, int bits);
extern void get_size(char * out, size_t in);
extern void advance(void * tok, const size_t n, FILE * fp);
