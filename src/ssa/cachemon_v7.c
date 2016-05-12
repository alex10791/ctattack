#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"


/*
#include <linux/module.h>      
#include <linux/kernel.h>      
#include <linux/init.h>         
#include <linux/hardirq.h>
#include <linux/preempt.h>
#include <linux/sched.h>
*/


#define RAM_ACCESS_TIME_EMPIRICAL 299
#define CACHE_LINE_DISTANCES 16

int main(int argc, char* argv[])
{

    unsigned long int tt = 0, tt2 = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
    unsigned long int x = 0;

    volatile char *tmp1;
    volatile char **tmp2;
    volatile char **tmp3;
    
    char ch = '\0';
    FILE *fp;

    size_t mem_length = (size_t)2*CACHE_L3_SIZE; 
    int mem_length_char = ((int)mem_length/sizeof(char));
    int mem_length_ptr = (int)mem_length/sizeof(void *);


    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char *C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char *D = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    // brings some data in Exclusive (OR Shared state - provably exclusive)
    x = B[MB(0)];
    x = B[MB(2)];
    x = B[MB(4)];
    x = C[MB(0)];
    x = C[MB(2)];
    x = D[MB(0)];
    x = D[MB(2)];
    //x = B[MB(4)/8+3*CACHE_L3_SET_OFFSET/8];




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

    for (int j = 0; j < 100000; ++j) {
        for (int k = 0; k < (int)mem_length; k+=CACHE_L3_SET_OFFSET) {
            x += D[k+CACHE_LINE_DISTANCES*CACHE_LINE];
        }

        for (int i = 0; i < CACHE_L3_ASSOCIATIVITY; ++i) {
            x += D[CACHE_L3_SET_OFFSET*(CACHE_L3_ASSOCIATIVITY+i)];
            for (int k = 1; k < (int)mem_length; k+=CACHE_L3_SET_OFFSET) {
                x += D[k+CACHE_LINE_DISTANCES*CACHE_LINE];
            }
        }
    }

*/




    // This is used to render the stride prefetcher useless... hopefully
    volatile char *B_arr[24];
    B_arr[0]  = B + 4*CACHE_L3_SET_OFFSET;
    B_arr[1]  = B + 9*CACHE_L3_SET_OFFSET;
    B_arr[2]  = B + 2*CACHE_L3_SET_OFFSET;
    B_arr[3]  = B + 1*CACHE_L3_SET_OFFSET;
    B_arr[4]  = B + 3*CACHE_L3_SET_OFFSET;
    B_arr[5]  = B + 6*CACHE_L3_SET_OFFSET;
    B_arr[6]  = B + 11*CACHE_L3_SET_OFFSET;
    B_arr[7]  = B + 5*CACHE_L3_SET_OFFSET;
    B_arr[8]  = B + 8*CACHE_L3_SET_OFFSET;
    B_arr[9]  = B + 10*CACHE_L3_SET_OFFSET;
    B_arr[10] = B + 7*CACHE_L3_SET_OFFSET;
    B_arr[11] = B + 0*CACHE_L3_SET_OFFSET;

    B_arr[12]  = B + 16*CACHE_L3_SET_OFFSET;
    B_arr[13]  = B + 21*CACHE_L3_SET_OFFSET;
    B_arr[14]  = B + 14*CACHE_L3_SET_OFFSET;
    B_arr[15]  = B + 13*CACHE_L3_SET_OFFSET;
    B_arr[16]  = B + 15*CACHE_L3_SET_OFFSET;
    B_arr[17]  = B + 18*CACHE_L3_SET_OFFSET;
    B_arr[18]  = B + 23*CACHE_L3_SET_OFFSET;
    B_arr[19]  = B + 17*CACHE_L3_SET_OFFSET;
    B_arr[20]  = B + 20*CACHE_L3_SET_OFFSET;
    B_arr[21]  = B + 22*CACHE_L3_SET_OFFSET;
    B_arr[22] = B + 29*CACHE_L3_SET_OFFSET;
    B_arr[23] = B + 12*CACHE_L3_SET_OFFSET;

/*
    B_arr[12]  = B + 13*CACHE_L3_SET_OFFSET;
    B_arr[13]  = B + 15*CACHE_L3_SET_OFFSET;
    B_arr[14]  = B + 23*CACHE_L3_SET_OFFSET;
    B_arr[15]  = B + 16*CACHE_L3_SET_OFFSET;
    B_arr[16]  = B + 21*CACHE_L3_SET_OFFSET;
    B_arr[17]  = B + 18*CACHE_L3_SET_OFFSET;
    B_arr[18]  = B + 17*CACHE_L3_SET_OFFSET;
    B_arr[19]  = B + 14*CACHE_L3_SET_OFFSET;
    B_arr[20]  = B + 29*CACHE_L3_SET_OFFSET;
    B_arr[21]  = B + 22*CACHE_L3_SET_OFFSET;
    B_arr[22] = B + 20*CACHE_L3_SET_OFFSET;
    B_arr[23] = B + 12*CACHE_L3_SET_OFFSET;
*/
    volatile char *C_arr[24];
    C_arr[0]  = C + 4*CACHE_L3_SET_OFFSET;
    C_arr[1]  = C + 9*CACHE_L3_SET_OFFSET;
    C_arr[2]  = C + 2*CACHE_L3_SET_OFFSET;
    C_arr[3]  = C + 1*CACHE_L3_SET_OFFSET;
    C_arr[4]  = C + 3*CACHE_L3_SET_OFFSET;
    C_arr[5]  = C + 6*CACHE_L3_SET_OFFSET;
    C_arr[6]  = C + 11*CACHE_L3_SET_OFFSET;
    C_arr[7]  = C + 5*CACHE_L3_SET_OFFSET;
    C_arr[8]  = C + 8*CACHE_L3_SET_OFFSET;
    C_arr[9]  = C + 10*CACHE_L3_SET_OFFSET;
    C_arr[10] = C + 7*CACHE_L3_SET_OFFSET;
    C_arr[11] = C + 0*CACHE_L3_SET_OFFSET;

    C_arr[12]  = C + 16*CACHE_L3_SET_OFFSET;
    C_arr[13]  = C + 21*CACHE_L3_SET_OFFSET;
    C_arr[14]  = C + 14*CACHE_L3_SET_OFFSET;
    C_arr[15]  = C + 13*CACHE_L3_SET_OFFSET;
    C_arr[16]  = C + 15*CACHE_L3_SET_OFFSET;
    C_arr[17]  = C + 18*CACHE_L3_SET_OFFSET;
    C_arr[18]  = C + 23*CACHE_L3_SET_OFFSET;
    C_arr[19]  = C + 17*CACHE_L3_SET_OFFSET;
    C_arr[20]  = C + 20*CACHE_L3_SET_OFFSET;
    C_arr[21]  = C + 22*CACHE_L3_SET_OFFSET;
    C_arr[22] = C + 29*CACHE_L3_SET_OFFSET;
    C_arr[23] = C + 12*CACHE_L3_SET_OFFSET;





    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;


    for (int i = 0; i < 12; ++i) {
        tmp1 = B_arr[i];
        TIMESTAMP_START;
        x += *tmp1;
        //x += B[i];
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        TIMESTAMP_START;
        TIMESTAMP_STOP;
        begin2 = get_global_timestamp_start();
        end2 = get_global_timestamp_stop();
        tt += (end-begin)-(end2-begin2);
        printf("%p: %lu\n", tmp1, (end-begin)-(end2-begin2) );
    }

    printf("\n");
/*
    for (int i = 0; i < 8; i+=1) {
        tmp1 = C_arr[i]+CACHE_L2_SET_OFFSET;
        TIMESTAMP_START;
        x += *tmp1;
        //x += C[i];
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        TIMESTAMP_START;
        TIMESTAMP_STOP;
        begin2 = get_global_timestamp_start();
        end2 = get_global_timestamp_stop();
        tt += (end-begin)-(end2-begin2);
        printf("%p: %lu\n", tmp1, (end-begin)-(end2-begin2) );
    }
*/

//    for (int i = 0; i < 1000; ++i) 
    //x += C[0]; 
    //printf("%p\n", &(C[0]) );


//for (int j = 0; j < 4; ++j) {
    printf("\n");

    tt2 = 0;
    for (int i = 0; i < 12; i+=1) {
        tmp1 = B_arr[i];
        TIMESTAMP_START;
        x += *tmp1;
        //x += B[i];
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        TIMESTAMP_START;
        TIMESTAMP_STOP;
        begin2 = get_global_timestamp_start();
        end2 = get_global_timestamp_stop();
        tt2 += (end-begin)-(end2-begin2);
        printf("%p: %lu\n", tmp1, (end-begin)-(end2-begin2) );
    }
//}

    //x += D[0];
    x += C[0];
    x += C[12*CACHE_L3_SET_OFFSET];

    printPtr2bin(C);
    printPtr2bin(&(C[12*CACHE_L3_SET_OFFSET]));

    printf("\n");

    for (int i = 11; i >= 0; i-=1) {
        tmp1 = B_arr[i];
        TIMESTAMP_START;
        x += *tmp1;
        //x += B[i];
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        TIMESTAMP_START;
        TIMESTAMP_STOP;
        begin2 = get_global_timestamp_start();
        end2 = get_global_timestamp_stop();
        tt2 += (end-begin)-(end2-begin2);
        printf("%p: %lu\n", tmp1, (end-begin)-(end2-begin2) );
    }


//preempt_disable();
//raw_local_irq_save(flags);
//raw_local_irq_restore(flags);
//preempt_enable();
    
    printf(" tt: %lu\n", tt );
    printf("tt2: %lu\n", tt2 );

    TIMESTAMP_START;
    TIMESTAMP_STOP;


    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();


    printf("%lu\n", end-begin);


    printf("Stride: %d\n", CACHE_L3_SET_OFFSET);
    


    return 0;
}



