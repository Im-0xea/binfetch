#pragma once

const struct spr mach_types[] = {
	{.key = 0x1, .str = "Object"},
	{.key = 0x2, .str = "Static"},
	{.key = 0x3, .str = "FVM Library"},
	{.key = 0x4, .str = "Core Dump"},
	{.key = 0x5, .str = "Preload"},
	{.key = 0x6, .str = "Dynamic Library"},
	{.key = 0x7, .str = "Dynamic Linker"},
	{.key = 0x8, .str = "Bundle"},
	{.key = 0x9, .str = "Dynamic Library Stub"},
	{.key = 0xa, .str = "Debug Symbols"},
	{.key = 0xb, .str = "Kernel Extension Bundle"},
};
