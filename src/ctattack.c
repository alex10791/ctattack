
#include "ctattack.h"

// IMPLEMENTATION

unsigned int timestamp(void) {
	unsigned int bottom;
	unsigned int top;
	asm volatile(".byte 15;.byte 49" : "=a"(bottom),"=d"(top));
	return bottom;
}


void ptr2bin(void* ptr, char bin[65]) {
	unsigned long int ptr_int = (unsigned long int)ptr;
	for (int i = 0; i < 64; ++i) {
		if (ptr_int & 0x1) bin[i] = '1'; else bin[i] = '0';
		ptr_int = ptr_int >> 1;
	}
	bin[64] = '\0';
}

void printPtr2bin(void* ptr) {
	char bin[65];
	ptr2bin((void *)ptr, bin);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			printf("%c", bin[63-(i*8+j)]);
		}
		printf(" ");
	}
	printf("\n");
}
