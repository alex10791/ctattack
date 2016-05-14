#ifndef __CTATTACK__
#define __CTATTACK__

#include <stdio.h>

// only one should be left uncommented
//#define MY_MACBOOK_PRO
//#define DTU_LIBRARY_PC_FEB2016
//#define DTU_LAB305_PC_FEB2016
#define DTU_NEHALEM
// up to here


#ifdef MY_MACBOOK_PRO

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

#define CACHE_L1_SET_OFFSET 4096		// 4K
#define CACHE_L2_SET_OFFSET 32768		// 32K
#define CACHE_L3_SET_OFFSET 262144/2	// 256K

//3MB / 12ways = 1MB / 4ways = 256KB/way


#define CACHE_LINE_MASK 0x3F

#define CACHE_L1_SET_MASK 0xFC0
#define CACHE_L2_SET_MASK 0x7FC0
#define CACHE_L3_SET_MASK 0x3FFC0

#define CACHE_L1_SIZE_MASK 0xFFF
#define CACHE_L2_SIZE_MASK 0x7FFF
#define CACHE_L3_SIZE_MASK 0x3FFFF

#endif


#ifdef DTU_LIBRARY_PC_FEB2016

#define CACHE_LINE 64
#define CACHE_L1_SIZE 32768				// 32k Data Cache
#define CACHE_L2_SIZE 262144			// 256k
#define CACHE_L3_SIZE 8388608			// 8M 3145728			// 3M
#define CACHE_L1_ASSOCIATIVITY 8
#define CACHE_L2_ASSOCIATIVITY 8
#define CACHE_L3_ASSOCIATIVITY 16
#define CACHE_LINE_BITS 6
#define CACHE_L1_SIZE_BITS 6
#define CACHE_L2_SIZE_BITS 9
#define CACHE_L3_SIZE_BITS 13

#define CACHE_L1_SET_OFFSET 4096
#define CACHE_L2_SET_OFFSET 32768
#define CACHE_L3_SET_OFFSET 524288


#define CACHE_LINE_MASK 0x3F

#define CACHE_L1_SET_MASK 0xFC0
#define CACHE_L2_SET_MASK 0x7FC0
#define CACHE_L3_SET_MASK 0x7FFC0

#define CACHE_L1_SIZE_MASK 0xFFF
#define CACHE_L2_SIZE_MASK 0x7FFF
#define CACHE_L3_SIZE_MASK 0x7FFFF

#define MEM_LOC_TE0	0x4029a0	
#define MEM_LOC_TE1	0x402da0	
#define MEM_LOC_TE2	0x4031a0	
#define MEM_LOC_TE3	0x4035a0	
#define MEM_LOC_TE4	0x4039a0	

#endif

#ifdef DTU_LAB305_PC_FEB2016

#define CACHE_LINE 64
#define CACHE_L1_SIZE 32768				// 32k Data Cache
#define CACHE_L2_SIZE 262144			// 256k
#define CACHE_L3_SIZE 6291456			// 6M 3145728			// 3M
#define CACHE_L1_ASSOCIATIVITY 8
#define CACHE_L2_ASSOCIATIVITY 8
#define CACHE_L3_ASSOCIATIVITY 12
#define CACHE_LINE_BITS 6
#define CACHE_L1_SIZE_BITS 6
#define CACHE_L2_SIZE_BITS 9
#define CACHE_L3_SIZE_BITS 13

#define CACHE_L1_SET_OFFSET 4096
#define CACHE_L2_SET_OFFSET 32768
#define CACHE_L3_SET_OFFSET 524288


#define CACHE_LINE_MASK 0x3F

#define CACHE_L1_SET_MASK 0xFC0
#define CACHE_L2_SET_MASK 0x7FC0
#define CACHE_L3_SET_MASK 0x7FFC0

#define CACHE_L1_SIZE_MASK 0xFFF
#define CACHE_L2_SIZE_MASK 0x7FFF
#define CACHE_L3_SIZE_MASK 0x7FFFF

#define MEM_LOC_TE0	0x4029a0	
#define MEM_LOC_TE1	0x402da0	
#define MEM_LOC_TE2	0x4031a0	
#define MEM_LOC_TE3	0x4035a0	
#define MEM_LOC_TE4	0x4039a0	

#endif


#ifdef DTU_NEHALEM

#define CACHE_LINE 64
#define CACHE_L1_SIZE 32768				// 32k Data Cache
#define CACHE_L2_SIZE 262144			// 256k
#define CACHE_L3_SIZE 4194304			// 4M
#define CACHE_L1_ASSOCIATIVITY 8
#define CACHE_L2_ASSOCIATIVITY 8
#define CACHE_L3_ASSOCIATIVITY 16
#define CACHE_LINE_BITS 6
#define CACHE_L1_SIZE_BITS 6
#define CACHE_L2_SIZE_BITS 9
#define CACHE_L3_SIZE_BITS 12

#define CACHE_L1_SET_OFFSET 4096		// 4K
#define CACHE_L2_SET_OFFSET 32768		// 32K
#define CACHE_L3_SET_OFFSET 262144/2	// 256K

//3MB / 12ways = 1MB / 4ways = 256KB/way


#define CACHE_LINE_MASK 0x3F

#define CACHE_L1_SET_MASK 0xFC0
#define CACHE_L2_SET_MASK 0x7FC0
#define CACHE_L3_SET_MASK 0x3FFC0

#define CACHE_L1_SIZE_MASK 0xFFF
#define CACHE_L2_SIZE_MASK 0x7FFF
#define CACHE_L3_SIZE_MASK 0x3FFFF

#endif


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




#define ROBOSTNESS_LOOP 10000 //1000000000
#define CACHE_L3_THRESHOLD 800000 //800000


// MARCROS

#define TIMESTAMP_START asm volatile (".align 16\n\t" "CPUID\n\t" "CPUID\n\t" "CPUID\n\t" "RDTSC\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t": "=r" (cycles_high_start), "=r" (cycles_low_start)::"%rax", "%rbx", "%rcx", "%rdx")
#define TIMESTAMP_STOP asm volatile ("RDTSCP\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "CPUID\n\t": "=r" (cycles_high_stop), "=r" (cycles_low_stop)::"%rax", "%rbx", "%rcx", "%rdx")
//#define TIMESTAMP_START asm volatile ("RDTSCP\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t": "=r" (cycles_high_start), "=r" (cycles_low_start)::"%rax", "%rbx", "%rcx", "%rdx")
//#define TIMESTAMP_STOP asm volatile ("RDTSCP\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t": "=r" (cycles_high_stop), "=r" (cycles_low_stop)::"%rax", "%rbx", "%rcx", "%rdx")

#define KB(x) x*1024
#define MB(x) x*1024*1024
//#define GB(x) x*1024*1024*1024



// GLOBAL VARIABLES

unsigned int cycles_low_start;
unsigned int cycles_high_start;
unsigned int cycles_low_stop;
unsigned int cycles_high_stop;



// DECLARATIONS

unsigned long int get_global_timestamp_start(void);
unsigned long int get_global_timestamp_stop(void);

unsigned int timestamp(void);
unsigned long int timestamp_start(void);
unsigned long int timestamp_stop(void);
unsigned int timestamp_weak(void);


void printPtr2bin(void* ptr);
void ptr2bin(void* ptr, char bin[65]);


/*
 *
 * SATTOLO ALGORITHM FUNCTIONS
 *
 */

static int rand_int(int n);
void sattolo_shuffle(volatile void **array, int n);


#endif
