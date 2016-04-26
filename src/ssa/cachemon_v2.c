
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

#define CONNECT

#define AVERAGE_REPS 100
#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)
#define CACHE_REPS 1

#define REPS 10

//#define CACHE_LINE_DISTANCES 1

int main(int argc, char* argv[])
{


    unsigned long int tt = 0;
    unsigned int begin, end;
    unsigned long int x = 0;
    FILE *fp;
    int CACHE_LINE_DISTANCES = 1;



    size_t mem_length = (size_t)CACHE_L3_SIZE; 

    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    //volatile char *B = mmap(NULL, (size_t)(2*(int)mem_length), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    volatile char *C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);



    printf("B\t%p\t\n", B);
    printPtr2bin((void*)B);
    
    printf("B\t%p\t\n", C);
    printPtr2bin((void*)C);





#ifdef CONNECT
    if (argc != 5) {
        printf("[!]Usage: %s IP PORT CACHE_LINE_DISTANCES OUTFILE.csv\n", argv[0]);
        return 1;
    }
    char *ip_str = argv[1];
    int server_port = atoi(argv[2]);


    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];


    char dir[256];
    sprintf(dir, "/home/user/Documents/github/ctattack/%s", argv[4]);
    fp = fopen((const char*)dir, "w");
    CACHE_LINE_DISTANCES = atoi(argv[3]);

#endif


#ifndef CONNECT
    if (argc != 3) {
        printf("[!]Usage: %s CACHE_LINE_DISTANCES OUTFILE.csv\n", argv[0]);
        return 1;
    }
    char *ip_str = argv[1];
    int server_port = atoi(argv[2]);


    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];


    char dir[256];
    sprintf(dir, "/home/user/Documents/github/ctattack/%s", argv[2]);
    fp = fopen((const char*)dir, "w");
    CACHE_LINE_DISTANCES = atoi(argv[1]);

#endif




    for (int count = 0; count < AVERAGE_REPS; ++count) {

    #ifdef CONNECT
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);

        if (socket_desc == -1) {
            printf("Could not create socket\n");
            return 1;
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
        begin2 = timestamp();
        x += C[0];
        x += C[MB(2)];
        x += C[MB(4)];
        x += B[0];
        x += B[MB(2)];
        x += B[MB(4)];
        x += B[MB(6)];
        end2 = timestamp();
        //printf("Load\t:\t%.8d cycles\n", end-begin);
        begin = timestamp();
        x += B[MB(2)-1];
        end = timestamp();
    //    printf("Disk\t:\t%.8d cycles\t%.8d cycles\n", end2-begin2, end-begin);


        for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
            for (int k = 0; k < CACHE_REPS*CACHE_L3_ASSOCIATIVITY; ++k) {
                C[i+CACHE_LINE_DISTANCES*CACHE_LINE] = 1;
            }
        }
        for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
            for (int k = 0; k < CACHE_REPS*CACHE_L3_ASSOCIATIVITY; ++k) {
                B[i+CACHE_LINE_DISTANCES*CACHE_LINE] = 1;
            }
        }

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


        begin = timestamp();
        for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
            for (int k = 0; k < CACHE_REPS*CACHE_L3_ASSOCIATIVITY; ++k) {
                x += B[i+CACHE_LINE_DISTANCES*CACHE_LINE]; 
            }
        }
        end = timestamp();

        //printf("Load\t:\t%.8d cycles\n", (end-begin)/( (long)CACHE_REPS*(long)CACHE_L3_ASSOCIATIVITY*(long)mem_length/CACHE_L3_SET_OFFSET) );

        for (int j = 0; j < REPS; ++j) {
            begin = timestamp();
            for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
                for (int k = 0; k < CACHE_REPS*CACHE_L3_ASSOCIATIVITY; ++k) {
                    x += B[i+CACHE_LINE_DISTANCES*CACHE_LINE]; 
                }
            }
            end = timestamp();
        //    printf("Load2\t:\t%.8d cycles\n", (end-begin)/( (long)CACHE_REPS*(long)CACHE_L3_ASSOCIATIVITY*(long)mem_length/CACHE_L3_SET_OFFSET) );
        }




    #ifdef CONNECT
        //Connect to remote server
        if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            printf("connection error\n");
            return 1;
        }
    #endif
       

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


        begin = timestamp();
        for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
            //for (int k = 0; k < CACHE_L3_ASSOCIATIVITY; ++k) {
                x += B[i+CACHE_LINE_DISTANCES*CACHE_LINE]; 
            //}
        }
        end = timestamp();
        //printf("Reload\t:\t%.8d cycles\t%d accesses\n", (end-begin)/( (int)mem_length/CACHE_L3_SET_OFFSET ), (int)mem_length/CACHE_L3_SET_OFFSET);

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


