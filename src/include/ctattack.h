#ifndef __CTATTACK__
#define __CTATTACK__

#include <stdio.h>

#define CACHE_LINE 64
#define CACHE_L1_SIZE 32768				// 32k Data Cache
#define CACHE_L2_SIZE 262144			// 256k
#define CACHE_L3_SIZE 3145728			// 3M
#define CACHE_L2_ASSOCIATIVITY 8

#define ROBOSTNESS_LOOP 10000
#define CACHE_L3_THRESHOLD 800000



// DECLARATIONS

unsigned int timestamp(void);


void printPtr2bin(void* ptr);
void ptr2bin(void* ptr, char bin[65]);


#endif
