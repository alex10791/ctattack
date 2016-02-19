// https://www.google.dk/search?client=ubuntu&channel=fs&q=openssl+aes+encryption+ecb&ie=utf-8&oe=utf-8&gws_rd=cr&ei=9ijFVqykKIuuswHBgIXYCA#channel=fs&q=openssl+aes+encryption+example+in+c
/*
 *
 * This server take an aritrary 16 byte input from the TCP stream and encrypt it using the
 * key key[] given below. The server will resond with the encrypted message and close the 
 * connection
 * 
 * Spawn the server like this:
 * ./aes_random_server PORT_NUMBER
 * 
 * the server will copy the first 16 bytes into a local array and clean the client message buffer
 * this might affect the cache performance and hence the chache timings
 *
 */


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <openssl/aes.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define VERBOSE
//#define TOUCH
#define RESPOND_WITH_BINARY
#define AES_BLOCK_SIZE_BYTES 16
#define AES_BLOCK_SIZE_BITS 128

static const unsigned char key[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

int main(int argc, char* argv[])
{

    if (argc != 2) {
        printf("[!]Usage: %s PORT\n", argv[0]);
        return 1;
    }
    int server_port = atoi(argv[1]);

    srand(time(NULL));

    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    
    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        printf("Could not create socket\n");
        //return 1;
    }

    printf("socket_desc: %d\n", socket_desc);


    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( server_port );


    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("bind failed\n");
    }
    printf("bind done\n");


    //Listen
    listen(socket_desc , 5);

    //Accept and incoming connection
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);

    
    unsigned char enc_out[AES_BLOCK_SIZE_BYTES];

    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {

        unsigned char *text;
        text = (unsigned char *)malloc(AES_BLOCK_SIZE_BYTES);
/*
        for (int i = 0; i < AES_BLOCK_SIZE_BYTES; ++i) {
            text[i] = '\0';
        }
*/

        for (int i = 0; i < AES_BLOCK_SIZE_BYTES; ++i) {
            text[i] = (char)rand()%256;                 //random chars
        }


        AES_KEY enc_key;

        AES_set_encrypt_key(key, AES_BLOCK_SIZE_BITS, &enc_key);
        AES_encrypt(text, enc_out, &enc_key);


#ifdef VERBOSE 
        //char *message;
        char message[64];
        unsigned char enc_str[240];
        //message = (char *) malloc( (strlen("Encrypted Ciphertext: ")) + strlen((char*)enc_out) * 3);
        sprintf(message, "Encrypted Ciphertext: ");

        enc_str[0] = '\0';
        
        for(int i=0;i<AES_BLOCK_SIZE_BYTES;i++) {
            sprintf((char*)enc_str, "%s%02X:", enc_str, enc_out[i]);
        }

        enc_str[strlen((char*)enc_str) - 1] = '\0';
        
        sprintf(message, "%s%s", message, enc_str);

        printf("%s\n", message);
        //free(message);
#endif 

#ifdef TOUCH
        puts("T");
#endif

#ifdef RESPOND_WITH_BINARY
        write(new_socket , enc_out , AES_BLOCK_SIZE_BYTES);
#endif

        free(text);
        close(new_socket);
    }



    if (new_socket<0)
    {
        perror("accept failed\n");
        return 1;
    }


    close(socket_desc);


    return 0;
}


