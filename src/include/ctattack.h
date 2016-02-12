#ifndef __CTATTACK__
#define __CTATTACK__

#include <stdio.h>

#define CACHE_LINE 64
#define CACHE_L1_SIZE 32768				// 32k Data Cache
#define CACHE_L2_SIZE 262144			// 256k
#define CACHE_L3_SIZE 3145728			// 3M
#define CACHE_L1_ASSOCIATIVITY 8
#define CACHE_L2_ASSOCIATIVITY 8
#define CACHE_L3_ASSOCIATIVITY 12
#define CACHE_LINE_BITS 6
#define CACHE_L1_SIZE_BITS 6
#define CACHE_L2_SIZE_BITS 9
#define CACHE_L3_SIZE_BITS 12

#define CACHE_L1_SET_OFFSET 4096
#define CACHE_L2_SET_OFFSET 32768
#define CACHE_L3_SET_OFFSET 262144


#define CACHE_LINE_MASK 0x3F

#define CACHE_L1_SET_MASK 0xFC0
#define CACHE_L2_SET_MASK 0x7FC0
#define CACHE_L3_SET_MASK 0x3FFC0

#define CACHE_L1_SIZE_MASK 0xFFF
#define CACHE_L2_SIZE_MASK 0x7FFF
#define CACHE_L3_SIZE_MASK 0x3FFFF


/*
 *
 * Core i5 (I5-2435M)
 * 
 * L1 Data/Instr Cache:
 * 32KB
 * 8sets x 4KB
 * 8sets x 64byte/line x 64B
 *
 * L2 Cache:
 * 256KB
 * 8sets x 32KB
 * 8sets x 64byte/line x 512B
 * 
 * L3 Cache: 
 * 3MB
 * 12sets x 256KB
 * 12sets x 64byte/line x 4KB
 * 
 */

#define ROBOSTNESS_LOOP 200 //1000000000
#define CACHE_L3_THRESHOLD 400000 //800000



// DECLARATIONS

unsigned int timestamp(void);


void printPtr2bin(void* ptr);
void ptr2bin(void* ptr, char bin[65]);


#endif
