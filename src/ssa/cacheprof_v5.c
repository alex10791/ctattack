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


    volatile char *B[64]; // = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    
    for (int i = 0; i < 64; ++i) {
        B[i] = mmap(NULL, (size_t)MB(2), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
        x += B[i][0];
    }



    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;


    for (int i = 0; i < 16; ++i) {
        TIMESTAMP_START;
        x += B[i][0];
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        TIMESTAMP_START;
        TIMESTAMP_STOP;
        begin2 = get_global_timestamp_start();
        end2 = get_global_timestamp_stop();
        tt += (end-begin)-(end2-begin2);
        printf("%p: %lu\n", B[i], (end-begin)-(end2-begin2) );
    }

    printf("\n");

    x += B[63][0];

    for (int i = 0; i < 16; i+=1) {
        TIMESTAMP_START;
        x += B[i][0];
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        TIMESTAMP_START;
        TIMESTAMP_STOP;
        begin2 = get_global_timestamp_start();
        end2 = get_global_timestamp_stop();
        tt2 += (end-begin)-(end2-begin2);
        printf("%p: %lu\n", B[i], (end-begin)-(end2-begin2) );
    }


    
    printf(" tt: %lu\n", tt );
    printf("tt2: %lu\n", tt2 );

    TIMESTAMP_START;
    TIMESTAMP_STOP;


    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();


    printf("%lu\n", end-begin);



    return 0;
}



