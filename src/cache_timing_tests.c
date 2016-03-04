
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

//#define CONNECT

#define AVERAGE_REPS 100
#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)

#define REPS 5

int main(int argc, char* argv[])
{

    unsigned long int tt = 0;
    unsigned int begin, end;
    unsigned long int x = 0;


//    int fd1 = fopen("/mnt/hgfs", "rw");
//    int fd2 = fopen("/mnt/hgfs", "rw");

    size_t mem_length = (size_t)MB(8); //CACHE_L3_SIZE;  //3*1024*1024; // 4MB : 10000 00000000 00000000

//    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB, -1, 0);
    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *B2 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *B3 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *B4 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *B5 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *B6 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *B7 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *B8 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
//    volatile char *B_off = B + (0x403a40 & 0xFFF);
    
//    volatile char *C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB, -1, 0);
    volatile char *C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *C2 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *C3 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *C4 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *C5 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *C6 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *C7 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    // volatile char *C8 = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    volatile char *D = mmap(NULL, 2*mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // x += B2[i];      // Takes less time for reload without CONNECT
    // x += B3[i];      // Takes less time for reload without CONNECT
    // x += B4[i];      // Takes less time for reload without CONNECT
    // x += B5[i];      // Takes less time for reload without CONNECT
    // x += B6[i];      // Takes less time for reload without CONNECT
    // x += B7[i];      // Takes less time for reload without CONNECT
    // x += B8[i];      // Takes less time for reload without CONNECT


/*    
    volatile char *B, *C;
    B=malloc((int)mem_length);
    C=malloc((int)mem_length);
*/
    printf("B\t%p\t\n", B);
    printPtr2bin((void*)B);
    
    printf("B\t%p\t\n", C);
    printPtr2bin((void*)C);


//for (int count = 0; count < 1000; ++count) {
    unsigned int begin2, end2;
    begin2 = timestamp();
    x += C[0];
    x += C[MB(2)];
    x += C[MB(4)];
    x += C[MB(6)];
    x += D[0];
    x += D[MB(2)];
    x += D[MB(4)];
    x += D[MB(6)];
    x += B[0];
    x += B[MB(2)];
    x += B[MB(4)];
    x += B[MB(6)];
    end2 = timestamp();
    //printf("Load\t:\t%.8d cycles\n", end-begin);
    begin = timestamp();
    x += B[MB(2)-1];
    end = timestamp();
    printf("Disk\t:\t%.8d cycles\t%.8d cycles\n", end2-begin2, end-begin);

/*
    for (int j = 0; j < 1000000; ++j) {
        for (int k = 0; k < (int)mem_length; k+=CACHE_L3_SET_OFFSET) {
            x += D[k];
        }

        for (int i = 0; i < CACHE_L3_ASSOCIATIVITY; ++i) {
            x += D[CACHE_L3_SET_OFFSET*(CACHE_L3_ASSOCIATIVITY+i)];
            for (int k = 1; k < (int)mem_length; k+=CACHE_L3_SET_OFFSET) {
                x += D[k];
            }
        }
    }
*/

    begin = timestamp();
    for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
        x += B[i];      // Takes less time for reload without CONNECT
        //B[i] = 1;     // Takes more time for reload without CONNECT
        //printf("%d\n", i);
    }
    // for (int j = 0; j < CACHE_L1_SIZE; j+=CACHE_L1_SET_OFFSET) {
    //     x += B[j];
    // }
    end = timestamp();
    printf("Load\t:\t%.8d cycles\n", end-begin);


    for (int j = 0; j < REPS; ++j) {
        begin = timestamp();
        for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
            x += B[i];      // Takes less time for reload without CONNECT
            //B[i] = 1;     // Takes more time for reload without CONNECT
        }
        // for (int j = 0; j < CACHE_L1_SIZE; j+=CACHE_L1_SET_OFFSET) {
        //     x += B[j];
        // }
        //x += B[0];
        end = timestamp();
        printf("Load2\t:\t%.8d cycles\n", end-begin);
    }

    for (int j = 0; j < 1; ++j) {
        begin = timestamp();
        for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
            x += C[i];      // Takes less time for reload without CONNECT
        }
        // for (int j = 0; j < CACHE_L1_SIZE; j+=CACHE_L1_SET_OFFSET) {
        //     x += C[j];
        // }
        end = timestamp();
        printf("Victim\t:\t%.8d cycles\n", end-begin);
    }
    begin = timestamp();
    for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
        x += B[i];      // Takes less time for reload without CONNECT
        //B[i] = 1;     // Takes more time for reload without CONNECT
    }
    // for (int j = 0; j < CACHE_L1_SIZE; j+=CACHE_L1_SET_OFFSET) {
    //     x += B[j];
    // }
    //x += B[0];
    end = timestamp();
    printf("Reload\t:\t%.8d cycles\t%d accesses\n", end-begin, (int)mem_length/CACHE_L3_SET_OFFSET);


    printf("CACHE_L1_SIZE/CACHE_L1_SET_OFFSET:\t%d accesses\n", (int)CACHE_L1_SIZE/CACHE_L1_SET_OFFSET);
    printf("CACHE_L1_SIZE/CACHE_L1_SET_OFFSET:\t%d accesses\n", (int)CACHE_L2_SIZE/CACHE_L2_SET_OFFSET);
    printf("CACHE_L3_SIZE/CACHE_L3_SET_OFFSET:\t%d accesses\n", (int)CACHE_L3_SIZE/CACHE_L3_SET_OFFSET);
    printf("mem_length/CACHE_L3_SET_OFFSET:\t\t%d accesses\n", (int)mem_length/CACHE_L3_SET_OFFSET);

    //while(1);
    return 0;


}

