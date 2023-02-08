#include "pair.h"

const bpr mach_arches[] =
{
	{
		.key = 0x7,
		.str = "i386"
	},
	
	{
		.key = 0x10000007,
		.str = "x86_64"
	},
	
	{
		.key = 0x12,
		.str = "armv7"
	},
	
	{
		.key = 0x1000000c,
		.str = "armv8"
	},
	
	{
		.key = 0x14,
		.str = "PowerPC"
	},
	
	{
		.key = 0x1000000f,
		.str = "PowerPC64"
	},
};
