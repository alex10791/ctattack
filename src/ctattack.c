
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





