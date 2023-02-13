#include <openssl/evp.h>

static void checksum_art(FILE * fp, const EVP_MD * type)
{
	int i, bytes;
	
	EVP_MD_CTX *ctx;
	ctx = EVP_MD_CTX_new();
	
	EVP_DigestInit_ex(ctx, type, NULL);
	
	unsigned char buf[1024], data[64];
	
	while ((bytes = fread (buf, 1, 1024, fp)) != 0)
	{
		EVP_DigestUpdate(ctx, buf, bytes);
	}
	
	unsigned int s;
	
	EVP_DigestFinal(ctx, data, &s);
	EVP_MD_CTX_free(ctx);
	
	i = 0;
	for(i = 0; i < 64; i++)
	{
		sprintf(&ascii_art[i / 8][i % 8 * 2],"%02x", data[i]);
		int len = strlen(ascii_art[i / 8]);
		if (len > max_line)
		{
			max_line = len;
		}
	}
	i = 0;
	
	rewind(fp);
}
