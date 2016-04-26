#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "unistd.h"
#include "inttypes.h"

#include "ctattack.h"

#define RAM_ACCESS_TIME_EMPIRICAL 70
#define HUGEPAGE_COUNT 8


int forceDectectWay(volatile char **arr[], volatile char **res[]);


int main(int argc, char* argv[])
{


    unsigned long int tt = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
    unsigned long int x = 0;
    //unsigned long int arr[CACHE_L3_SIZE/8];
    char ch = '\0';

    int arr[8];
    arr[0] = 1;

    size_t mem_length = (size_t)(4*CACHE_L3_SIZE);      //MB(2)/8;      //CACHE_L3_SIZE;
    volatile char **B[HUGEPAGE_COUNT];
    volatile char **C[CACHE_L3_ASSOCIATIVITY];
    
    volatile char **buff;
    
    //for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
    //    B[i] = (volatile char **)mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    //}

    buff = (volatile char **)mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    
    printf("Base:\t%p\n", buff);


    for (int i = 0; i < (unsigned long int)mem_length/sizeof(volatile char*); ++i) {
        //buff[i] = (volatile char *)&buff[(i+1)%((unsigned long int)mem_length/sizeof(volatile char*))]; 
        buff[i] = (volatile char *)(buff + i);
    }
    //printf("%p\n", buff[((unsigned long int)mem_length/sizeof(volatile char*))-1]);


    begin = timestamp();
    buff = (volatile char**)*buff;
    end = timestamp();
    printf("%lu\n", end-begin);

    scanf("%c", &ch);

    //sattolo_shuffle((volatile void **)buff, (unsigned long int)mem_length/sizeof(volatile char*));



    for (int i = 0; i < (unsigned long int)mem_length/sizeof(volatile char*); ++i) {
        begin = timestamp();
        buff = (volatile char**)*buff;
        end = timestamp();
        //printf("%p\n", buff[0]);
        printf("A%luA\n", end-begin);
        scanf("%c", &ch);
    }


    return 0;





















    // fetch from disk and print virtual addresses
    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        B[i][1] = i;
        printf("%d: ", i);
        printf("B\t%p\t\n", B[i]);
        printPtr2bin((void*)B[i]);
    }

    // fetch from disk
    tt = 0;
    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        x += (unsigned long int)B[i][1];
    }





    printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
//    forceDectectWay(B, C);


    volatile char *tmp;
    volatile char *tmp2;

/*
    for (int j = 0; j < 4; ++ j) {
        printf("##################### %d #####################\n", j);
        for (int i = 0; i < CACHE_L3_ASSOCIATIVITY; ++i) {
            begin=timestamp();
            tmp = (volatile char **)C[i];
            end=timestamp();
            if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
                printf("F: RAM");
            } else {
                printf("F: L3");
            }
            printf("\t: %lu\n", end-begin);
        }
        //tmp = (volatile char **)B[63];
    }
*/    


    //return 0;


    //shuffle((volatile void **)B, HUGEPAGE_COUNT);

    tmp = (volatile char *)B[0];

    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        //x += (unsigned long int)B[i][1];

        begin=timestamp();
        //tmp = *B[i];        //(volatile char **)
        tmp += *(volatile char *)B[i];
        end=timestamp();

//        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
//            printf("F: RAM");
//        } else {
//            printf("F: L3");
//        }
        printf("\t: %lu\n", end-begin);

//        begin=timestamp();
//        tmp2 = (volatile char **)B[0];
//        end=timestamp();
        //arr[i] = end-begin;
//        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
//            printf("S: RAM");
//        } else {
//            printf("S: L3");
//        }
//        printf("\t: %lu\n", (end-begin));
        tt += end-begin;
    }
    printf("%lu\n", tt/HUGEPAGE_COUNT);


}












int forceDectectWay(volatile char **arr[], volatile char **res[]) {
	unsigned long int tt = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
    unsigned long int x = 0;
    volatile char **tmp;
    volatile char **tmp2;
	
    res[0] = arr[0];

    int k = 1;    

	for (int i = 1; i < HUGEPAGE_COUNT; ++i) {
        printf("%d\n", i);
        // add new cache way
        tmp = (volatile char **)arr[i];
        // bring in all assigned cache ways
		for (int j = 0; i < k; ++k) {
            tmp2 = (volatile char **)res[j];
		}
        // time eviction time
        begin=timestamp();
        tmp = (volatile char **)arr[i];
        end=timestamp();
        if ((end-begin) <= RAM_ACCESS_TIME_EMPIRICAL) {
            res[k] = arr[i];
            ++k;
        }
        if (k == CACHE_L3_ASSOCIATIVITY) {
            return 1;
        }
	}
    return 0;
}







/*

	tt = 0;
    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        x += (unsigned long int)B[i][1];
    }

    //shuffle((volatile void **)B, HUGEPAGE_COUNT);

    volatile char **tmp;

    for (int i = 0; i < HUGEPAGE_COUNT; ++i) {
        x += (unsigned long int)B[i][1];

        begin=timestamp();
		tmp = (volatile char **)B[0];
        end=timestamp();
        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
        	printf("F: RAM");
        } else {
        	printf("F: L3");
        }
        printf("\t: %lu\n", end-begin);

        begin=timestamp();
        B[0] = (volatile char **)B[0];
        end=timestamp();
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

*/
