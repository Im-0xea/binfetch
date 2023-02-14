#include <stdbool.h>
#include <string.h>
#include "info.h"

extern label labels[128];
extern int   label_count;

void add_label(char * name, char * out)
{
	strcpy(labels[label_count].key, name);
	labels[label_count].out = out;
	labels[label_count++].used = false;
}
