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


    volatile char **B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char **C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char **D = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    // brings some data in Exclusive (OR Shared state - provably exclusive)
    x = B[MB(0)/8+3*CACHE_L3_SET_OFFSET/8];
    x = B[MB(2)/8+3*CACHE_L3_SET_OFFSET/8];
    //x = B[MB(4)/8+3*CACHE_L3_SET_OFFSET/8];



    // brings data in Modified state
    // this should take some time since most data is NOT in the cache yet
    /*
    for (int i = 0; i < CACHE_L3_SIZE; i+=CACHE_L3_SET_OFFSET) {
        begin = timestamp_start();
        B[i] = 1;
        end = timestamp_stop();
        tt += end-begin;
    }
    */

    volatile char **E;

    B[0] = (volatile char *)(B+CACHE_L3_SET_OFFSET/8);
    B[CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+5*CACHE_L3_SET_OFFSET/8);
    B[5*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+7*CACHE_L3_SET_OFFSET/8);
    B[7*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+9*CACHE_L3_SET_OFFSET/8);
    B[9*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+3*CACHE_L3_SET_OFFSET/8);
    B[3*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+11*CACHE_L3_SET_OFFSET/8);
    B[11*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+10*CACHE_L3_SET_OFFSET/8);
    B[10*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+2*CACHE_L3_SET_OFFSET/8);
    B[2*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+8*CACHE_L3_SET_OFFSET/8);
    B[8*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+6*CACHE_L3_SET_OFFSET/8);
    B[6*CACHE_L3_SET_OFFSET/8] = (volatile char *)(B+4*CACHE_L3_SET_OFFSET/8);


    C[0] = (volatile char *)(C+CACHE_L3_SET_OFFSET/8);
    C[CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+2*CACHE_L3_SET_OFFSET/8);
    C[2*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+3*CACHE_L3_SET_OFFSET/8);
    C[3*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+4*CACHE_L3_SET_OFFSET/8);
    C[4*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+5*CACHE_L3_SET_OFFSET/8);
    C[5*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+6*CACHE_L3_SET_OFFSET/8);
    C[6*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+7*CACHE_L3_SET_OFFSET/8);
    C[7*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+8*CACHE_L3_SET_OFFSET/8);
    C[8*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+9*CACHE_L3_SET_OFFSET/8);
    C[9*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+10*CACHE_L3_SET_OFFSET/8);
    C[10*CACHE_L3_SET_OFFSET/8] = (volatile char *)(C+11*CACHE_L3_SET_OFFSET/8);


    D[0] = (volatile char *)(D+CACHE_L3_SET_OFFSET/8);
    D[CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+2*CACHE_L3_SET_OFFSET/8);
    D[2*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+3*CACHE_L3_SET_OFFSET/8);
    D[3*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+4*CACHE_L3_SET_OFFSET/8);
    D[4*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+5*CACHE_L3_SET_OFFSET/8);
    D[5*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+6*CACHE_L3_SET_OFFSET/8);
    D[6*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+7*CACHE_L3_SET_OFFSET/8);
    D[7*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+8*CACHE_L3_SET_OFFSET/8);
    D[8*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+9*CACHE_L3_SET_OFFSET/8);
    D[9*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+10*CACHE_L3_SET_OFFSET/8);
    D[10*CACHE_L3_SET_OFFSET/8] = (volatile char *)(D+11*CACHE_L3_SET_OFFSET/8);



     for (int j = 0; j < 1000; ++j) {
        E = D;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
    }


    for (int i = 0; i < 1000000; ++i) {

        E = B;


        TIMESTAMP_START;
        //begin = timestamp_start();
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        //end = timestamp_stop();

        E = B;

        //begin2 = timestamp_start();
        TIMESTAMP_START;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        E = (volatile char **)*E;
        TIMESTAMP_STOP;
        begin2 = get_global_timestamp_start();
        end2 = get_global_timestamp_stop();
        //end2 = timestamp_stop();



        tt += end-begin;
        tt2 += end2-begin2;

}

    printf("%d\n", tt);
    printf("%d\n", tt2);



    return 0;
}



