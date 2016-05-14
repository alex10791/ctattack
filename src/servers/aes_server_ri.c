// https://www.google.dk/search?client=ubuntu&channel=fs&q=openssl+aes+encryption+ecb&ie=utf-8&oe=utf-8&gws_rd=cr&ei=9ijFVqykKIuuswHBgIXYCA#channel=fs&q=openssl+aes+encryption+example+in+c
/*
 *
 * This server take an aritrary 16 byte input from the TCP stream and encrypt it using the
 * key key[] given below. The server will resond with the encrypted message and close the 
 * connection
 * 
 * Spawn the server like this:
 * ./aes_server PORT_NUMBER
 * 
 * the server will copy the first 16 bytes into a local array and clean the client message buffer
 * this might affect the cache performance and hence the chache timings
 *
 */


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
//#include <openssl/aes.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

#include "rijndael-alg-fst.h"

#define SERVER_TCP_BUFFER_SIZE 2000
#define AES_BLOCK_SIZE_BYTES 16
#define AES_BLOCK_SIZE_BITS 128

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
    printf("VIRT PTR Te0: %p\n", get_virt_addr_TeN(0));
    printf("VIRT PTR Te1: %p\n", get_virt_addr_TeN(1));
    printf("VIRT PTR Te2: %p\n", get_virt_addr_TeN(2));
    printf("VIRT PTR Te3: %p\n", get_virt_addr_TeN(3));
    printf("VIRT PTR Te4: %p\n", get_virt_addr_TeN(4));
    printf("PHY PAGE INDEX PTR Te0: %p\n", get_phy_addr_TeN(0));
    printf("PHY PAGE INDEX PTR Te1: %p\n", get_phy_addr_TeN(1));
    printf("PHY PAGE INDEX PTR Te2: %p\n", get_phy_addr_TeN(2));
    printf("PHY PAGE INDEX PTR Te3: %p\n", get_phy_addr_TeN(3));
    printf("PHY PAGE INDEX PTR Te4: %p\n", get_phy_addr_TeN(4));
    printf("PHY PTR Te0: %p\n", (void *)(((unsigned long int)get_phy_addr_TeN(0) << 12) | ((unsigned long int)get_virt_addr_TeN(0) & 0xFFF)));
    printf("PHY PTR Te1: %p\n", (void *)(((unsigned long int)get_phy_addr_TeN(1) << 12) | ((unsigned long int)get_virt_addr_TeN(1) & 0xFFF)));
    printf("PHY PTR Te2: %p\n", (void *)(((unsigned long int)get_phy_addr_TeN(2) << 12) | ((unsigned long int)get_virt_addr_TeN(2) & 0xFFF)));
    printf("PHY PTR Te3: %p\n", (void *)(((unsigned long int)get_phy_addr_TeN(3) << 12) | ((unsigned long int)get_virt_addr_TeN(3) & 0xFFF)));
    printf("PHY PTR Te4: %p\n", (void *)(((unsigned long int)get_phy_addr_TeN(4) << 12) | ((unsigned long int)get_virt_addr_TeN(4) & 0xFFF)));

    if (argc != 2) {
        printf("[!]Usage: %s PORT\n", argv[0]);
        return 1;
    }
    int server_port = atoi(argv[1]);


    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *message, client_message[SERVER_TCP_BUFFER_SIZE];

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
    listen(socket_desc , 3);

    //Accept and incoming connection
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);


    volatile unsigned long int x = 0;
    size_t mem_length = (size_t)MB(2);
    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    B[2048] = 0xAA;


    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {

//        printf("Connection accepted from IP:%s Port:%d\n", inet_ntoa(client.sin_addr), client.sin_port);
        //Reply to the client
        
        
        unsigned char *text;
        text = (unsigned char *)malloc(80);
        unsigned char enc_out[80];
        unsigned char enc_str[240];


        for (int i = 0; i < 80; ++i) {
            text[i] = '\0';
            enc_out[i] = '\0';
        }

        message = (char *) malloc( (strlen("Encrypted Ciphertext: ") + 16 * 3));
        //message = (char *) malloc( (strlen("Encrypted Ciphertext: ")) + strlen((char*)enc_out) * 3);
        //sprintf(message, "Encrypted Ciphertext: ");
        enc_str[0] = '\0';


        u32 enc_key[16];
        rijndaelKeySetupEnc(enc_key, (const u8*) key, AES_BLOCK_SIZE_BITS);



        recv(new_socket, client_message , SERVER_TCP_BUFFER_SIZE , 0); 
        //client_message[strlen(client_message)] = '\0';

        
        int len = 16;
        for (int i = 0; i < len + 1; ++i) {
            text[i] = (unsigned char)client_message[i];
            client_message[i] = '\0';
        }

/*
        printf("TEXT: ");
        for (int i = 0; i < 16; ++i) {
            printf("%02x::", text[i]);
        }
        printf("\n");

        printf(" KEY: ");
        for (int i = 0; i < 16; ++i) {
            printf("%02x::", key[i]);
        }
        printf("\n");
*/

/*
        AES_KEY enc_key;
        AES_set_encrypt_key(key, 128, &enc_key);
        AES_encrypt(text, enc_out, &enc_key);
*/

        //x += (unsigned long int)(*(B+2048) ^ 0x12);

        x += (get_TeN_idx(4, 0) & (0xFF<<8)) >> 8;

        //rijndaelEncrypt(enc_key, 10, text, enc_out);

/*
        printf(" ENC: ");
        //for(int i=0;*(enc_out+i)!=0x00;i++) {
        for(int i=0; i < 16; i++) {
            printf("%02X::", enc_out[i]);
            sprintf((char*)enc_str, "%s%02X:", enc_str, enc_out[i]);
        }
        printf("\n");
*/

//        printf("\n");

//        enc_str[strlen((char*)enc_str) - 1] = '\0';
//        sprintf(message, "%s%s", message, enc_str);

//        printf("%s\n", message);
//        write(new_socket , message , strlen(message));
        write(new_socket , enc_out , 16);





        free(message);
        free(text);
        close(new_socket);
    }

    if (new_socket<0)
    {
        perror("accept failed\n");
        return 1;
    }


    close(socket_desc);





    unsigned char text[]="hello world!";
    unsigned char enc_out[80];
    unsigned char dec_out[80];

//    AES_KEY enc_key, dec_key;

//    AES_set_encrypt_key(key, 128, &enc_key);
//    AES_encrypt(text, enc_out, &enc_key);      

//    AES_set_decrypt_key(key,128,&dec_key);
//    AES_decrypt(enc_out, dec_out, &dec_key);

    int i;

    printf("original:\t");
    for(i=0;*(text+i)!=0x00;i++)
        printf("%X ",*(text+i));
    printf("\nencrypted:\t");
    for(i=0;*(enc_out+i)!=0x00;i++)
        printf("%X ",*(enc_out+i));
    printf("\ndecrypted:\t");
    for(i=0;*(dec_out+i)!=0x00;i++)
        printf("%X ",*(dec_out+i));
    printf("\n");

    return 0;
}


