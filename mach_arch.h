#include "pair.h"

const bpr mach_arches[] =
{
	{
		.key = 0x00000007,
		.str = "i386"
	},
	
	{
		.key = 0x01000007,
		.str = "x86_64"
	},
	
	{
		.key = 0x00000012,
		.str = "armv7"
	},
	
	{
		.key = 0x0100000c,
		.str = "armv8"
	},
	
	{
		.key = 0x00000014,
		.str = "PowerPC"
	},
	
	{
		.key = 0x0100000f,
		.str = "PowerPC64"
	},
};
