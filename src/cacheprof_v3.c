
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "unistd.h"
#include "inttypes.h"

#include "ctattack.h"

#define RAM_ACCESS_TIME_EMPIRICAL 70
#define HUGEPAGE_COUNT 64

static int rand_int(int n);
void shuffle(volatile void **array, int n);
uintptr_t vtop(uintptr_t vaddr);
unsigned long get_page_frame_number_of_address(void *addr);


int main(int argc, char* argv[])
{


    unsigned long int tt = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
    unsigned long int x = 0;
    //unsigned long int arr[CACHE_L3_SIZE/8];
    char ch = '\0';

    int arr[CACHE_L3_SIZE/8];
    arr[0] = 1;

    size_t mem_length = (size_t)32*CACHE_L3_SIZE;       //MB(2);		//CACHE_L3_SIZE;
    volatile char **B;

    

    B = (volatile char **)mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);



    for (int i = 0; i < CACHE_L3_SIZE/8; ++i) {
        //printf("%d\n", i);
    	B[i] = (void **)B+i+1;
    	arr[i] = i;
    }

    //scanf("%c", &ch);
    

    printf("B\t%p\t\n", B);
	printPtr2bin((void*)B);
	//printf("B\t%p\t\n", (void*)pB);
    //printPtr2bin((void*)pB);
	//printf("B\t%p\t\n", (void*)pB2);
    //printPtr2bin((void*)pB2);


    
    tt = 0;
    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        x += (unsigned long int)B[i][1];
    }

    //shuffle((volatile void **)B, HUGEPAGE_COUNT);

    shuffle(B, HUGEPAGE_COUNT);

    volatile char **tmp;

    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        //x += (unsigned long int)B[i][1];

        /*
        printf("%p\n", B);
        begin=timestamp();
        tmp = (volatile char **)B[0];
        end=timestamp();
        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("F: RAM");
        } else {
            printf("F: L3");
        }
        printf("\t: %lu\n", end-begin);
        */
        begin=timestamp();
        B = (volatile char **)B[0];
        /*
        x += (unsigned long int)B[i][1]<<1;
        x += (unsigned long int)B[i][1]<<2;
        x += (unsigned long int)B[i][1]<<3;
        x += (unsigned long int)B[i][1]<<4;
        x += (unsigned long int)B[i][1]<<5;
        x += (unsigned long int)B[i][1]<<6;
        x += (unsigned long int)B[i][1]<<7;
        x += (unsigned long int)B[i][1]<<8;
        x += (unsigned long int)B[i][1]<<9;
        */
        end=timestamp();
        
        //x += (unsigned long int)B[i+1][1];

        //arr[i] = end-begin;
        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end-begin));
        tt += end-begin;
    }
    printf("%lu\n", tt/HUGEPAGE_COUNT);

    printf("TEST2\n");

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
    int offset = (vaddr / 4096 ) * sizeof(uint64_t);			// sysconf(_SC_PAGESIZE)	//MB(2)
    uint64_t e;

    // https://www.kernel.org/doc/Documentation/vm/pagemap.txt
    if ((pagemap = fopen("/proc/self/pagemap", "r"))) {
    	printf("First if\n");
        if (lseek(fileno(pagemap), offset, SEEK_SET) == offset) {
    		printf("Second if\n");
            //if (fread(&e, sizeof(uint64_t), 1, pagemap)) {
    		if ((e = getc(pagemap))) {
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


