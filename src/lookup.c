#include "lookup.h"
#include <stdio.h>

#include <linux/input.h>


char table[256];

int lookup_init(char* filename){
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("Failed to open file");
		return 1;
	}
	fread(table, 1, 256, fp);
	fclose(fp);
	return 0;
}

char lookup_keycode(lookup_key k){
	return table[k];
}
