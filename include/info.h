#pragma once

struct label {
	char key[128];
	char * out;
	bool used;
};

extern void add_label(char * name, char * out);
