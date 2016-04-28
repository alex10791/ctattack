
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "ctattack.h"

//#define VERBOSE
#define AES_BLOCK_SIZE_BYTES 16
#define SERVER_TCP_BUFFER_SIZE 2000

int main(int argc, char* argv[])
{

    if (argc != 3) {
        printf("[!]Usage: %s IP PORT\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    char *ip_str = argv[1];
    int server_port = atoi(argv[2]);


    int socket_desc;
    struct sockaddr_in server;
    char message[32];
    char server_reply[2000];


    server.sin_addr.s_addr = inet_addr(ip_str);     //80.239.174.120
    server.sin_family = AF_INET;
    server.sin_port = htons( server_port );


    int stats[256];


    int encryptions = 200;


    for (int k = 0; k < encryptions; ++k) {

        for (int i = 0; i < 10000000; ++i);

        socket_desc = socket(AF_INET, SOCK_STREAM, 0);


        if (socket_desc == -1) {
            printf("Could not create socket\n");
            continue;
            //return -1;
        }

    #ifdef VERBOSE 
        printf("socket_desc: %d\n", socket_desc);
    #endif

        

        //Connect to remote server
        if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            printf("connection error\n");
            //return 1;
            continue;
        }
        
    #ifdef VERBOSE 
        printf("Connected\n");
    #endif


        //message = "GET / HTTP/1.1\r\n\r\n";
        for (int i = 0; i < AES_BLOCK_SIZE_BYTES; ++i) {
            message[i] = (char)rand()%256;
        }


        if( send(socket_desc , message , AES_BLOCK_SIZE_BYTES , 0) < 0)
        {
            puts("Send failed");
            continue;
            //return 1;
        }

    #ifdef VERBOSE 
        puts("Data Send\n");
    #endif



        int resp_size = 0;
        resp_size = recv(socket_desc, server_reply , SERVER_TCP_BUFFER_SIZE , 0);

        for (int i = 0; i < 16; ++i) {
            printf("%d ", server_reply[i]);
        }
        printf("%s\n", server_reply+16);

        if( resp_size < 0)
        {
            puts("recv failed");
            continue;
            //return -1;
        }

    #ifdef VERBOSE 
        puts("Reply received\n");
    #endif

        server_reply[resp_size] = '\0';
        //printf("%d\n", strlen(server_reply));
        printf("tt: %d\n", atoi(server_reply+16));
        stats[server_reply[0]]++;
        //printf("%s", server_reply);

        close(socket_desc);

    }

    

}

