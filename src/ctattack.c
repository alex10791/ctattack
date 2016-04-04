
#include "ctattack.h"

#include <stdlib.h>

// IMPLEMENTATION

unsigned int timestamp_weak(void) {
	unsigned int bottom;
	unsigned int top;
	asm volatile(".byte 15;.byte 49" : "=a"(bottom),"=d"(top));
	return bottom;
}

unsigned int timestamp(void) {
	//unsigned int bottom;
	//unsigned int top;
	unsigned int cycles_low;
	unsigned int cycles_high;
	//asm volatile(".byte 15;.byte 49" : "=a"(bottom),"=d"(top));
	asm volatile ("CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t"
		"CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	return cycles_low;
}

unsigned long int timestamp_start(void) {
	//unsigned int bottom;
	//unsigned int top;
	unsigned int cycles_low;
	unsigned int cycles_high;
	//asm volatile(".byte 15;.byte 49" : "=a"(bottom),"=d"(top));
	asm volatile ("CPUID\n\t"
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	return ((unsigned long int)cycles_high << 32) | cycles_low;
}

unsigned long int timestamp_stop(void) {
	//unsigned int bottom;
	//unsigned int top;
	unsigned int cycles_low;
	unsigned int cycles_high;
	//asm volatile(".byte 15;.byte 49" : "=a"(bottom),"=d"(top));
	asm volatile ("RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t"
		"CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	return ((unsigned long int)cycles_high << 32) | cycles_low;
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






/*
 *
 * SATTOLO ALGORITHM FUNCTIONS
 *
 */

static int rand_int(int n) {
    int limit = RAND_MAX - RAND_MAX % n;
    int rnd;
    do {
        rnd = rand();
    } 
    while (rnd >= limit);
    return rnd % n;
}

 

void sattolo_shuffle(volatile void **array, int n) {
    int i, j;
    volatile void **tmp;
    for (i = n - 1; i > 0; i--) {
        j = rand_int(i + 1);
        tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
   }

}

