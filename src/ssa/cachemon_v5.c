#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"


#define RAM_ACCESS_TIME_EMPIRICAL 299
#define CACHE_LINE_DISTANCES 16

int main(int argc, char* argv[])
{


    unsigned long int tt = 0, tt2 = 0, tt3 = 0, tt4 = 0, tt5 = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
    unsigned long int begin3, end3;
    unsigned long int begin4, end4;
    unsigned long int begin5, end5;
    unsigned long int x = 0;

    volatile char **tmp1;
    volatile char **tmp2;
    volatile char **tmp3;
    
    char ch = '\0';
    FILE *fp;

    size_t mem_length = (size_t)CACHE_L3_SIZE; 
    int mem_length_char = ((int)mem_length/sizeof(char));
    int mem_length_ptr = (int)mem_length/sizeof(void *);


    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char *C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char *D = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    // brings some data in Exclusive (OR Shared state - provably exclusive)
    x = B[MB(0)+3*CACHE_L3_SET_OFFSET];
    x = B[MB(2)+3*CACHE_L3_SET_OFFSET];
    x = B[MB(4)+3*CACHE_L3_SET_OFFSET];


    timestamp_start();
    timestamp_stop();
    timestamp_start();
    timestamp_stop();
    

    unsigned long int tbias = 0;
    for (int i = 0; i < 10000; ++i) {
        begin2 = timestamp_start();
        end2 = timestamp_stop();
        tbias += (end2 - begin2);
    }
    tbias /= 10000;
    printf("tbias: %d\n", tbias);

for (int abc = 0; abc < 100; ++abc){
    /*
     *
     * Need to setup this part for Fix-Policy if possible
     * Attempt to get LRU policy
     * Seems to Somehow Work
     * Withouth results in 1 mode around 600 clockcycles
     * With 10000 results in 2 modes around 600 clockcycles and 1000 clockcycles
     * With 100000 results in 1 mode around 1000 clockcycles
     *
     */
/*
    for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
        for (int j = 0; j < (int)mem_length; j+=CACHE_L3_SET_OFFSET) {
            x += C[j+i];
        }
     }
*/
    for (int j = 0; j < 100000; ++j) {
        for (int k = 0; k < (int)mem_length; k+=CACHE_L3_SET_OFFSET) {
            x += C[k+CACHE_LINE_DISTANCES*CACHE_LINE];
        }

        for (int i = 0; i < CACHE_L3_ASSOCIATIVITY; ++i) {
            x += C[CACHE_L3_SET_OFFSET*(CACHE_L3_ASSOCIATIVITY+i)];
            for (int k = 1; k < (int)mem_length; k+=CACHE_L3_SET_OFFSET) {
                x += C[k+CACHE_LINE_DISTANCES*CACHE_LINE];
            }
        }
    }






    // brings data in Modified state
    // this should take some time since most data is NOT in the cache yet
    for (int i = 0; i < CACHE_L3_SIZE; i+=CACHE_L3_SET_OFFSET) {
        /*
        printf("cacheline: %0.8d ------------------------------------------------------\n", i);
        for (int j = 0; j < 64; j+=32) {
            begin = timestamp_start();
            B[i+j] = 1;
            end = timestamp_stop();
            printf("%d:%d\t", i+j, end-begin-tbias);
        }
        printf("\n");
        */
        begin = timestamp_start();
        B[i] = 1;
        end = timestamp_stop();
        tt += end-begin;
    }


    // accesses data from Modified state
    // this should be rather fast since data are already in cache
    for (int i = 0; i < CACHE_L3_SIZE; i+=CACHE_L3_SET_OFFSET) {
        /*
        printf("cacheline: %0.8d ------------------------------------------------------\n", i);
        for (int j = 0; j < 64; j+=32) {
            begin = timestamp_start();
            B[i+j] = 1;
            end = timestamp_stop();
            printf("%d:%d\t", i+j, end-begin-tbias);
        }
        printf("\n");
        */
        begin = timestamp_start();
        x += B[i];
        end = timestamp_stop();
        tt2 += end-begin;
    }


    x += D[0];
//    x += D[1];
//    x += D[2];
//    x += D[3];

    // accesses data from Modified state
    // this should be rather fast since data are already in cache
    for (int i = 0; i < CACHE_L3_SIZE; i+=CACHE_L3_SET_OFFSET) {
        /*
        printf("cacheline: %0.8d ------------------------------------------------------\n", i);
        for (int j = 0; j < 64; j+=32) {
            begin = timestamp_start();
            B[i+j] = 1;
            end = timestamp_stop();
            printf("%d:%d\t", i+j, end-begin-tbias);
        }
        printf("\n");
        */
        begin = timestamp_start();
        x += B[i];
        end = timestamp_stop();
        tt3 += end-begin;
    }
}
    printf("%d\n", tt);
    printf("%d\n", tt2);
    printf("%d\n", tt3);


    printf("tbias: %d\n", tbias);

    return 0;
}



