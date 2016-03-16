
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "unistd.h"
#include "inttypes.h"

#include "ctattack.h"


static int rand_int(int n);
void shuffle(volatile void **array, int n);
uintptr_t vtop(uintptr_t vaddr);
unsigned long get_page_frame_number_of_address(void *addr);


int main(int argc, char* argv[])
{


    unsigned long int tt = 0;
    unsigned int begin, end;
    unsigned int begin2, end2;
    unsigned long int x = 0;
    unsigned long int arr[CACHE_L3_SIZE/8];
    char ch = '\0';

    size_t mem_length = (size_t)CACHE_L3_SIZE;
    volatile void **B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    for (int i = 0; i < CACHE_L3_SIZE/8; ++i) {
    	B[i] = (void **)B+i;
    	arr[i] = i;
    }

    


    uintptr_t pB = vtop((uintptr_t)B);
    unsigned long pB2 = get_page_frame_number_of_address((void *)B);

    printf("B\t%p\t\n", B);
	printPtr2bin((void*)B);
	printf("B\t%p\t\n", (void*)pB);
    printPtr2bin((void*)pB);
	//printf("B\t%p\t\n", (void*)pB2);
    //printPtr2bin((void*)pB2);




    shuffle(B, CACHE_L3_SIZE/8);

	x = (long int)B;

	begin2=timestamp();
    for (int i = 0; i < CACHE_L3_SIZE/8; ++i) {
    	begin=timestamp();
	    B = (void **)*B;
	    end=timestamp();
	    arr[i] = end-begin;
    }
    end2=timestamp();


    printf("%lu\n", end2-begin2);
    for (int i = 0; i < (CACHE_L3_SIZE/8)/16; ++i) {
    	for (int j = 0; j < 16; ++j) {
    		printf("%lu\n", arr[i*16+j]);
    	}
		scanf("%c", &ch);
    }

}




#define PAGEMAP_LENGTH MB(2)

unsigned long get_page_frame_number_of_address(void *addr) {
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






uintptr_t vtop(uintptr_t vaddr) {
    FILE *pagemap;
    intptr_t paddr = 0;
    printf("%d", MB(2));														// sysconf(_SC_PAGESIZE))
    int offset = (vaddr / MB(2)) * sizeof(uint64_t);							// sysconf(_SC_PAGESIZE))
    uint64_t e;

    // https://www.kernel.org/doc/Documentation/vm/pagemap.txt
    if ((pagemap = fopen("/proc/self/pagemap", "r"))) {
    	printf("First if\n");
        if (lseek(fileno(pagemap), offset, SEEK_SET) == offset) {
    		printf("Second if\n");
            //if (fread(&e, sizeof(uint64_t), 1, pagemap)) {
    		if (e = getc(pagemap)) {
    			printf("Third if\n");
                if (e & (1ULL << 63)) { // page present ?
    				printf("Fourth if\n");
                    paddr = e & ((1ULL << 54) - 1); // pfn mask
                    paddr = paddr * MB(2);										// sysconf(_SC_PAGESIZE))
                    // add offset within page
                    paddr = paddr | (vaddr & (MB(2) - 1));						// sysconf(_SC_PAGESIZE))
                }   
            }   
        }   
        fclose(pagemap);
    }   

    return paddr;
} 



static int rand_int(int n) {
        int limit = RAND_MAX - RAND_MAX % n;
        int rnd;
        do {
            rnd = rand();
        } 
        while (rnd >= limit);
        return rnd % n;
    }

     

    void shuffle(volatile void **array, int n) {
        int i, j;
        volatile void **tmp;
        for (i = n - 1; i > 0; i--) {
            j = rand_int(i + 1);
            tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
       }

    }
