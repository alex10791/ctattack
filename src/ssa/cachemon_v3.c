#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

//#define CONNECT

#define RAM_ACCESS_TIME_EMPIRICAL 299


static int rand_int(int n);
void sattolo(volatile void **array, int n);

int main(int argc, char* argv[])
{


    unsigned long int tt = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
    unsigned long int begin3, end3;
    unsigned long int begin4, end4;
    unsigned long int begin5, end5;
    unsigned long int x = 0;
    volatile char **tmp;
    char ch = '\0';
    FILE *fp;
    int CACHE_LINE_DISTANCES = 1;


    size_t mem_length = (size_t)6*CACHE_L3_SIZE; 
    int mem_length_char = ((int)mem_length/sizeof(char));
    int mem_length_ptr = (int)mem_length/sizeof(void *);






    volatile char **B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char **C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


 //   x = (volatile char **)B[0];
 //   x = (volatile char **)B[MB(2)/8];
 //   x = (volatile char **)B[MB(4)/8];
 //   x = (volatile char **)B[MB(6)/8];

 //   x = (volatile char **)B[MB(8)/8];
 //   x = (volatile char **)B[MB(10)/8];

    for (int i = 0; i < mem_length_ptr; ++i) {
        //B[i] = (volatile char *)((volatile char **)B+i);
        B[i] = (volatile char *)((volatile char **)B+i+1);
        //printf("%p\n", B[i]);
        //if (i%8 == 7) {
        //    B[i] = (volatile char *)&x;
        //}
    }

/*
    sattolo((volatile void **)B, mem_length_ptr);

    for (int i = 0; i < mem_length_char; ++i) {
        //printf("%p\n", B[i]);
        if ((i % (CACHE_LINE/8)) == 0) {
            printf("##############################\n");
        }
        printPtr2bin(B[i]);
    }

    return 0;
*/

/*    x = B[MB(12)/8];
    x = B[MB(14)/8];

    x = B[MB(16)/8];
    x = B[MB(18)/8];
    x = B[MB(20)/8];
    x = B[MB(22)/8];

    x = B[MB(24)/8];
    x = B[MB(26)/8];
    x = B[MB(28)/8];
    x = B[MB(30)/8];
*/

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
            tmp = (volatile char **)C[k+CACHE_LINE_DISTANCES*CACHE_LINE];                                                              //+CACHE_LINE_DISTANCES*CACHE_LINE
        }

        for (int i = 0; i < CACHE_L3_ASSOCIATIVITY; ++i) {
            tmp = (volatile char **)C[(CACHE_L3_SET_OFFSET/8)*(CACHE_L3_ASSOCIATIVITY+i)];
            for (int k = 1; k < (int)mem_length/8; k+=CACHE_L3_SET_OFFSET/8) {
                tmp = (volatile char **)C[k+CACHE_LINE_DISTANCES*CACHE_LINE];                                                          //+CACHE_LINE_DISTANCES*CACHE_LINE
            }
        }
    }



    printf("%p\n", B[0+2*CACHE_LINE]);
    printf("%p\n", &B[0+2*CACHE_LINE+1]);
    
    for (int i = 0; i < mem_length_char/8; i+=CACHE_L3_SET_OFFSET/8) {           // CACHE_LINE
        printf("%d #############\n", i/(CACHE_L3_SET_OFFSET/8));




        begin = timestamp_start();
        tmp = (volatile char **)B[i+2*CACHE_LINE/8];
        /*tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp += 1;*/
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
        tmp = (volatile char **)B[i+2*CACHE_LINE/8];
        /*tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp += 1;*/
        end2 = timestamp_stop();



//        begin4 = timestamp_start();
//        tmp = (volatile char **)B[i+2*CACHE_L2_SET_OFFSET/8+2*CACHE_LINE/8];
        /*tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp += 1;*/
//        end4 = timestamp_stop();


//        begin5 = timestamp_start();
//        tmp = (volatile char **)B[i+2*CACHE_LINE/8];
        /*tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp += 1;*/
//        end5 = timestamp_stop();


        begin3 = timestamp_start();
        end3 = timestamp_stop();
        //end3 = begin3;


        //printPtr2bin((void*)&B[i+2*CACHE_LINE/8]);

        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end-begin) - (end3-begin3));

        if ((end2-begin2) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end2-begin2) - (end3-begin3));
/*
        if ((end4-begin4) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end4-begin4) - (end3-begin3));

        if ((end5-begin5) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("S: RAM");
        } else {
            printf("S: L3");
        }
        printf("\t: %lu\n", (end5-begin5) - (end3-begin3));
*/
    }


    //tmp = (volatile char **)C[0];
    //tmp1 = (volatile char **)C[30*CACHE_LINE];
    for (int i = 0; i < 1000; ++i) {
        for (int i = 0; i < mem_length_char/8; i+=CACHE_L3_SET_OFFSET/8) {
            //tmp = (volatile char **)*tmp;
            x += (unsigned long int)C[i+30*CACHE_LINE/8];
            //tmp1 = (volatile char **)C[30*CACHE_LINE];
        }
    }



    for (int i = 0; i < mem_length_char/8; i+=CACHE_L3_SET_OFFSET/8) {
        //printf("%p\n", &B[(mem_length_char/8)-(i+2*CACHE_LINE)-CACHE_L3_SET_OFFSET/8]);
        //printf("%d #############\n", i/(CACHE_L3_SET_OFFSET/8));
        begin = timestamp_start();
        //x = B[(mem_length_char/8)-(i+2*CACHE_LINE)-CACHE_L3_SET_OFFSET/8];
        tmp = (volatile char **)B[i+2*CACHE_LINE/8];                           //(mem_length_char/8)-(i+2*CACHE_LINE)-CACHE_L3_SET_OFFSET/8
        /*tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp = (volatile char **)*tmp;
        tmp += 1;*/
        end = timestamp_stop();

        begin3 = timestamp_start();
        end3 = timestamp_stop();
        //end3 = begin3;

        if ((end-begin) > RAM_ACCESS_TIME_EMPIRICAL) {
            printf("R: RAM");
        } else {
            printf("**************************** R: L3");
        }
        printf("\t: %lu\n", (end-begin) - (end3-begin3));
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

 

void sattolo(volatile void **array, int n) {
    int i, j;
    volatile void **tmp[CACHE_LINE/8];
    n /= CACHE_LINE/8;
    for (i = n - 1; i > 0; i--) {
        j = rand_int(i + 1);
        for (int k = 0; k < CACHE_LINE/8; ++k) {
            tmp[k] = array[k+j*CACHE_LINE/8];
            array[k+j*CACHE_LINE/8] = array[k+i*CACHE_LINE/8];
            array[k+i*CACHE_LINE/8] = tmp[k];
        }
   }

}

