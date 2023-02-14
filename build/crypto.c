#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>

extern char ascii_art[128][128];
extern int max_width;
extern int max_height;
extern int max_colors ;

const int cw = 10;

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
