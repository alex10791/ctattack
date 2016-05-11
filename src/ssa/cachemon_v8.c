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

    size_t mem_length = (size_t)MB(2); 
    int mem_length_char = ((int)mem_length/sizeof(char));
    int mem_length_ptr = (int)mem_length/sizeof(void *);


    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char *C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char *D = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    // brings some data in Exclusive (OR Shared state - provably exclusive)
    x = B[MB(0)];
    //x = B[MB(2)];
    //x = B[MB(4)];
    x = C[MB(0)];
    //x = C[MB(2)];
    //x = D[MB(0)];
    x = D[MB(0)];
    //x = B[MB(4)/8+3*CACHE_L3_SET_OFFSET/8];





    // This is used to render the stride prefetcher useless... hopefully
    volatile char *B_arr[24];
    B_arr[0]  = B + (0x00 << 17);
    B_arr[1]  = B + (0x03 << 17);
    B_arr[2]  = B + (0x04 << 17);
    B_arr[3]  = B + (0x07 << 17);
    B_arr[4]  = B + (0x09 << 17);
    B_arr[5]  = B + (0x0A << 17);
    B_arr[6]  = B + (0x0D << 17);
    B_arr[7]  = B + (0x0E << 17);

    B_arr[8]  = C + (0x00 << 17);
    B_arr[9]  = C + (0x03 << 17);
    B_arr[10] = C + (0x04 << 17);
    B_arr[11] = C + (0x07 << 17);
    B_arr[12] = C + (0x09 << 17);
    B_arr[13] = C + (0x0A << 17);
    B_arr[14] = C + (0x0D << 17);
    B_arr[15] = C + (0x0E << 17);
/*
    B_arr[8]  = B + (0x10 << 17);
    B_arr[9]  = B + (0x13 << 17);
    B_arr[10] = B + (0x14 << 17);
    B_arr[11]  = B + (0x17 << 17);
    B_arr[12]  = B + (0x19 << 17);
    B_arr[13]  = B + (0x1A << 17);
    B_arr[14]  = B + (0x1D << 17);
    B_arr[15]  = B + (0x1E << 17);
*/





    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;


    for (int i = 0; i < 13; ++i) {
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
        //printPtr2bin(B_arr[i]);
    }



    printf("\n");



    TIMESTAMP_START;
    //x += *D;
    //x += *(D + (1 << 17));
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
    printf("%p: %lu\n", D, (end-begin)-(end2-begin2) );


    printf("\n");



    for (int i = 0; i < 13; ++i) {
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

    TIMESTAMP_START;
    TIMESTAMP_STOP;


    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();


    printf("%lu\n", end-begin);

    return 0;
}



