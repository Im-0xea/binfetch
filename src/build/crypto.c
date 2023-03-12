#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <openssl/evp.h>

extern char ascii_art[128][128];
extern int max_width;
extern int max_height;
extern int max_colors ;

const int cw = 8;

void checksum_art(FILE * fp)
{
	const EVP_MD * type = EVP_sha512();
	int i, bytes;
	
	EVP_MD_CTX *ctx;
	ctx = EVP_MD_CTX_new();
	
	EVP_DigestInit_ex(ctx, type, NULL);
	
	unsigned char buf[1028], data[64], frt[64];
	
	while ((bytes = fread (buf, 1, 1028, fp)) != 0)
	{
		EVP_DigestUpdate(ctx, buf, bytes);
	}
	
	unsigned int s;
	
	EVP_DigestFinal(ctx, frt, &s);
	EVP_MD_CTX_free(ctx);
	for(i = 0; i < s; i++)
	{
		sprintf(&ascii_art[i / cw][i % cw * 2],"%02x", frt[i]);
		int len = strlen(ascii_art[i / cw]);
		if (len > max_width)
		{
			max_width = len;
		}
	}
	
	EVP_MD_CTX *ctx2;
	ctx2 = EVP_MD_CTX_new();
	
	EVP_DigestInit_ex(ctx2, type, NULL);
	EVP_DigestUpdate(ctx2, frt, 64);
	EVP_DigestFinal(ctx2, data, &s);
	EVP_MD_CTX_free(ctx2);
	
	max_height = (s * 2) / cw;
	
	for(; i < s * 2; i++)
	{
		sprintf(&ascii_art[i / cw][i % cw * 2],"%02x", data[i]);
		int len = strlen(ascii_art[i / cw]);
		if (len > max_width)
		{
			max_width = len;
		}
	}
	
	rewind(fp);
}

static int clamp(int min, int max, int in)
{
	return in < min ? min : in > max ? max : in;
}

void drunken_bishop(FILE * fp)
{
	const int w = 18;
	const int h = 10;
	const int it = 200;
	const char set[] = " .o+=*B0X@%&#/^SE";
	
	fseek(fp, 0L, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* file_data = (char*)malloc(file_size);
	fread(file_data, 1, file_size, fp);
	unsigned long in = crc32(0L, Z_NULL, 0);
	in = crc32(in, (const Bytef*)file_data, file_size);
	free(file_data);
	rewind(fp);
	
	srand(in);
	
	
	int map[256][256];
	for (int bz = 0; bz < h; ++bz)
	{
		bzero(map[bz], w);
	}
	
	int x = rand() % w, y = rand() % h;
	int i = 0;
	while (i < it)
	{
		switch (rand() % 4)
		{
			case 0: // up left
			{
				if (x > 0 && y < h - 1)
				{
					++map[++y][--x];
				}
				break;
			};
			case 1: // up right
			{
				if (x < w - 1 && y < h - 1)
				{
					++map[++y][++x];
				}
				break;
			};
			case 2: // down left
			{
				if (x > 0 && y > 0)
				{
					++map[--y][--x];
				}
				break;
			};
			case 3: // down right
			{
				if (x < w - 1 && y > 0)
				{
					++map[--y][++x];
				}
				break;
			};
		}
		++i;
	}
	
	max_width = w;
	max_height = h;
	for (y = 0; y < h; ++y)
	{
		for (x = 0; x < w; ++x)
		{
			ascii_art[y][x] = set[clamp(0, 16, map[y][x])];
		}
	}
	
	return;
}
