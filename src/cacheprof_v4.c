
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "unistd.h"
#include "inttypes.h"

#include "ctattack.h"

#define RAM_ACCESS_TIME_EMPIRICAL 300
#define HUGEPAGE_COUNT 64
#define CACHE_LINE_DISTANCES 1

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

    //int arr[HUGEPAGE_COUNT*MB(2)/8];
    //arr[0] = 1;

    size_t mem_length = (size_t)HUGEPAGE_COUNT*MB(2);       //MB(2);		//CACHE_L3_SIZE;
    volatile char **B;

    

    B = (volatile char **)mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);



    for (int i = 0; i < HUGEPAGE_COUNT*MB(2)/8; ++i) {
        //printf("%d\n", i);
    	B[i] = (void **)B+i+1;
        if (i%8 == 7) {                         //(i+1)%0x100 == 0x40
            //printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            B[i] = &end;
        }
        //printf("%p\t%p\n", B+i, B+i+1);
    	//arr[i] = i;
    }

    //scanf("%c", &ch);
    

    printf("B\t%p\t\n", B);
    printPtr2bin((void*)B);
    printf("##############################################\n");
	//printf("B\t%p\t\n", (void*)pB);
    //printPtr2bin((void*)pB);
	//printf("B\t%p\t\n", (void*)pB2);
    //printPtr2bin((void*)pB2);


    
    tt = 0;
    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        x += (unsigned long int)B[i][1];
    }
/*
    printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    shuffle((volatile void **)B, HUGEPAGE_COUNT*MB(2)/8);

    for (int i = 0; i < HUGEPAGE_COUNT*MB(2)/8; ++i) {
        printf("%p\n", &B[i]);
        scanf("%c", &ch);
    }
*/
    //shuffle(B, HUGEPAGE_COUNT);



    /*
     *
     * Need to setup this part for Fix-Policy if possible
     * Attempt to get LRU policy
     *
     */
/*
    for (int j = 0; j < 10000; ++j) {
        for (int k = 0; k < (unsigned long int)mem_length/sizeof(volatile char *); k+=CACHE_L3_SET_OFFSET) {
            x += B[k+CACHE_LINE_DISTANCES*CACHE_LINE];
        }

        for (int i = 0; i < CACHE_L3_ASSOCIATIVITY; ++i) {
            x += B[CACHE_L3_SET_OFFSET*(CACHE_L3_ASSOCIATIVITY+i)];
            for (int k = 1; k < (unsigned long int)mem_length/sizeof(volatile char *); k+=CACHE_L3_SET_OFFSET) {
                x += B[k+CACHE_LINE_DISTANCES*CACHE_LINE];
            }
        }
    }
*/

    

    volatile char **tmp;

    for (int i = 0; i < HUGEPAGE_COUNT*MB(2)/8; i+=CACHE_L3_SET_OFFSET/8) {
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
        //tmp = &begin;
        //*tmp=timestamp_start();
        begin=timestamp_start();
        //printPtr2bin((void*)&B[CACHE_L3_SET_OFFSET*i/8]);
        tmp = (volatile char **)B[i];                          // tmp = B[i]
/*        //printPtr2bin((void*)tmp);
        //printf("%p -> %p\n", tmp, *tmp);
        tmp = (volatile char **)*tmp;                           // tmp = B[i+1]
        //printPtr2bin((void*)tmp);
        //printf("%p -> %p\n", tmp, *tmp);
        tmp = (volatile char **)*tmp;                           // tmp = B[i+2]
        //printPtr2bin((void*)tmp);
        //printf("%p -> %p\n", tmp, *tmp);
        tmp = (volatile char **)*tmp;                           // tmp = B[i+3]
        //printPtr2bin((void*)tmp);
        //printf("%p -> %p\n", tmp, *tmp);
        tmp = (volatile char **)*tmp;                           // tmp = B[i+4]
        //printPtr2bin((void*)tmp);
        //printf("%p -> %p\n", tmp, *tmp);
        tmp = (volatile char **)*tmp;                           // tmp = B[i+5]
        //printPtr2bin((void*)tmp);
        //printf("%p -> %p\n", tmp, *tmp);
        tmp = (volatile char **)*tmp;                           // tmp = B[i+6]
        //printPtr2bin((void*)tmp);
        //printf("%p -> %p\n", tmp, *tmp);
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
*/
/*
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
        tmp = (volatile char **)*tmp;                           // tmp = B[i+7]
*/
        //tmp = &x;
        //printPtr2bin((void*)tmp);
        //printf("%p\n", tmp);
        //printf("%p -> %p\n", tmp, *tmp);

        //printf("********************\n");
        //*tmp += 1;//(unsigned long int)tmp[i][1];
        //printf("%p\n", tmp);
        //printf("%p\n", &x);
        //tmp = (volatile char **)*tmp;
        end=timestamp_stop();
        //*tmp=timestamp_stop();
        
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


    begin = timestamp_start();
    end = timestamp_stop();
    printf("%lu\n", (end-begin));


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
    //volatile void **tmp;
    volatile void *tmp[CACHE_LINE/sizeof(volatile char*)];
    for (i = n/sizeof(volatile char*) - 1; i > 0; i-=CACHE_LINE/sizeof(volatile char*)) {
        printf("i: %d\n", i);
        j = rand_int(i + 1);
        printf("j: %d\n", j);
        for (int k = 0; k < CACHE_LINE/sizeof(volatile char*); ++k) {
            printf("%d\n", k);
            tmp[k] = array[CACHE_LINE/sizeof(volatile char*)*j+k];
            array[CACHE_LINE/sizeof(volatile char*)*j+k] = array[CACHE_LINE/sizeof(volatile char*)*i+k];
            array[CACHE_LINE/sizeof(volatile char*)*i+k] = tmp[k];
        }
   }

}


