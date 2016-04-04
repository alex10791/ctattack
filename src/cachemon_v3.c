#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

//#define CONNECT

#define RAM_ACCESS_TIME_EMPIRICAL 300


int main(int argc, char* argv[])
{


    unsigned long int tt = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
//    unsigned long int x = 0;
    volatile char *x;
    char ch = '\0';
    FILE *fp;
    int CACHE_LINE_DISTANCES = 1;


    size_t mem_length = (size_t)4*CACHE_L3_SIZE; 
    int mem_length_char = ((int)mem_length/sizeof(char));
    int mem_length_ptr = (int)mem_length/sizeof(void *);






    volatile char **B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char **C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    x = B[0];
    x = B[MB(2)/8];
    x = B[MB(4)/8];
    x = B[MB(6)/8];

    x = B[MB(8)/8];
    x = B[MB(10)/8];
    x = B[MB(12)/8];
    x = B[MB(14)/8];

    x = B[MB(16)/8];
    x = B[MB(18)/8];
    x = B[MB(20)/8];
    x = B[MB(22)/8];

    x = B[MB(24)/8];
    x = B[MB(26)/8];
    x = B[MB(28)/8];
    x = B[MB(30)/8];


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


    for (int j = 0; j < 100000; ++j) {
        for (int k = 0; k < (int)mem_length/8; k+=CACHE_L3_SET_OFFSET/8) {
            x = C[k];                                                              //+CACHE_LINE_DISTANCES*CACHE_LINE
        }

        for (int i = 0; i < CACHE_L3_ASSOCIATIVITY; ++i) {
            x = C[(CACHE_L3_SET_OFFSET/8)*(CACHE_L3_ASSOCIATIVITY+i)];
            for (int k = 1; k < (int)mem_length/8; k+=CACHE_L3_SET_OFFSET/8) {
                x = C[k];                                                          //+CACHE_LINE_DISTANCES*CACHE_LINE
            }
        }
    }




    for (int i = 0; i < mem_length_char/8; i+=CACHE_L3_SET_OFFSET/8) {           // CACHE_LINE
        printf("%d #############\n", i/(CACHE_L3_SET_OFFSET/8));
        begin = timestamp_start();
        x = B[i+2*CACHE_LINE];
        end = timestamp_stop();
        /*
        printPtr2bin((void*)&B[i+2*CACHE_LINE]);
        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end-begin));
        */



        begin2 = timestamp_start();
        x = B[i+2*CACHE_LINE];
        end2 = timestamp_stop();


        printPtr2bin((void*)&B[i+2*CACHE_LINE]);
        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end-begin));

        if ((end2-begin2) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end2-begin2));

    }


    


    for (int i = 0; i < mem_length_char/8; i+=CACHE_L3_SET_OFFSET/8) {
        printf("%p\n", &B[(mem_length_char/8)-(i+2*CACHE_LINE)-CACHE_L3_SET_OFFSET/8]);
        //printf("%d #############\n", i/(CACHE_L3_SET_OFFSET/8));
        begin = timestamp_start();
        x = B[(mem_length_char/8)-(i+2*CACHE_LINE)-CACHE_L3_SET_OFFSET/8];
        end = timestamp_stop();
        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
        //    printf("R: RAM");
        } else {
        //    printf("**************************** R: L3");
        }
        //printf("\t: %lu\n", (end-begin));
    }






    printf("B\t%p\t\n", B);
    printPtr2bin((void*)B);
    
    printf("B\t%p\t\n", C);
    printPtr2bin((void*)C);


    begin = timestamp_start();
    end = timestamp_stop();
    printf("\t: %lu\n", (end-begin));


    return 0;
}