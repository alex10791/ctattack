
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char* argv[])
{

    if (argc != 3) {
        printf("[!]Usage: %s IP PORT\n", argv[0]);
        return 1;
    }
    char *ip_str = argv[1];
    int server_port = atoi(argv[2]);


    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        printf("Could not create socket\n");
        //return 1;
    }

    printf("socket_desc: %d\n", socket_desc);


    server.sin_addr.s_addr = inet_addr(ip_str);     //80.239.174.120
    server.sin_family = AF_INET;
    server.sin_port = htons( server_port );

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("connection error\n");
        return 1;
    }
     
    printf("Connected\n");





    message = "GET / HTTP/1.1\r\n\r\n";
    if( send(socket_desc , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");






    if( recv(socket_desc, server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }
    puts("Reply received\n");
    puts(server_reply);


    close(socket_desc);

}

