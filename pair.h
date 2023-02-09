#pragma once

typedef unsigned char byte;
typedef unsigned int fbyte;

typedef struct small_pair
{
	byte key;
	char str[64];
}
spr;

typedef struct big_pair
{
	fbyte key;
	char str[64];
}
bpr;
