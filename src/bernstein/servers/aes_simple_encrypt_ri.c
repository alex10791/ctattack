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
//#include <openssl/aes.h>
#include "rijndael-alg-fst.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <sys/mman.h>
#include "ctattack.h"


//#define VERBOSE
//#define TOUCH
#define RESPOND_WITH_BINARY
#define AES_BLOCK_SIZE_BYTES 16
#define AES_BLOCK_SIZE_BITS 128
#define SERVER_TCP_BUFFER_SIZE 2000

static const unsigned char key[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};
/*
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
*/

int main(int argc, char* argv[])
{

    if (argc != 2) {
        printf("[!]Usage: %s PORT\n", argv[0]);
        return 1;
    }

    unsigned long int begin, end;

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
        text = (unsigned char *)malloc(SERVER_TCP_BUFFER_SIZE);

        unsigned char *plaintext;
        plaintext = (unsigned char *)malloc(AES_BLOCK_SIZE_BYTES);



        //printf("Connection accepted from IP:%s Port:%d\n", inet_ntoa(client.sin_addr), client.sin_port);
        //Reply to the client
        
        recv(new_socket, text , SERVER_TCP_BUFFER_SIZE , 0);          
        //text[strlen(text)] = '\0';         
        text[SERVER_TCP_BUFFER_SIZE-1] = '\0';

        for (int i = 0; i < AES_BLOCK_SIZE_BYTES; ++i) {
            plaintext[i] = text[i];
            printf("%c", plaintext[i]);
        }
        printf("\n");


        //printf("text %s\n", text);
        //printf("plaintext %s\n", plaintext);

        //u32 enc_key[16];
        //printf("ACCESSING\n");



        u32 enc_key[16];
        ////rijndaelKeySetupEnc(u32 rk[/*4*(Nr + 1)*/], const u8 cipherKey[], int keyBits);
        rijndaelKeySetupEnc(enc_key, (const u8*) key, AES_BLOCK_SIZE_BITS);
        ////AES_set_encrypt_key(key, AES_BLOCK_SIZE_BITS, &enc_key);
        ////rijndaelKeySetupEnc(u32 rk[/*4*(Nr + 1)*/], const u8 cipherKey[], int keyBits);
        //rijndaelKeySetupEnc(enc_key, (const u8*) key, AES_BLOCK_SIZE_BITS);
        ////AES_set_encrypt_key(key, AES_BLOCK_SIZE_BITS, &enc_key);
        ////rijndaelEncrypt(const u32 rk[/*4*(Nr + 1)*/], int Nr, const u8 pt[16], u8 ct[16]);

        begin = timestamp_start();
        rijndaelEncrypt(enc_key, 10, plaintext, enc_out);
        end = timestamp_stop();

        char message[64];
        for (int i = 0; i < AES_BLOCK_SIZE_BYTES; ++i) {
            message[i] = enc_out[i];
        }
        message[AES_BLOCK_SIZE_BYTES] = '\0';
        unsigned long int tt = end-begin;
        /*
        tt = 0x4142434445464748;
        message[AES_BLOCK_SIZE_BYTES]   =  0x00000000000000ff & tt;
        message[AES_BLOCK_SIZE_BYTES+1] = (0x000000000000ff00 & tt) >> 8;
        message[AES_BLOCK_SIZE_BYTES+2] = (0x0000000000ff0000 & tt) >> 16;
        message[AES_BLOCK_SIZE_BYTES+3] = (0x00000000ff000000 & tt) >> 24;
        message[AES_BLOCK_SIZE_BYTES+4] = (0x000000ff00000000 & tt) >> 32;
        message[AES_BLOCK_SIZE_BYTES+5] = (0x0000ff0000000000 & tt) >> 40;
        message[AES_BLOCK_SIZE_BYTES+6] = (0x00ff000000000000 & tt) >> 48;
        message[AES_BLOCK_SIZE_BYTES+7] = (0xff00000000000000 & tt) >> 56;
        message[AES_BLOCK_SIZE_BYTES+8] = '\0';
        */
        printf("tt : %lu\n", tt);
        sprintf(message, "%s%lu", message, tt);
        printf("message: %s\n", message);
        //message[AES_BLOCK_SIZE_BITS] = (end - begin);
        ////AES_encrypt(text, enc_out, &enc_key);


#ifdef VERBOSE 
        //char *message;
        //char message[64];
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
        write(new_socket , message , AES_BLOCK_SIZE_BYTES+sizeof(unsigned long int));
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


