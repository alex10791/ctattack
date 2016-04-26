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
#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)


//#define VERBOSE
//#define TOUCH
#define RESPOND_WITH_BINARY
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

    volatile unsigned long int x = 0;
    size_t mem_length = (size_t)CACHE_L3_SIZE; // 4MB : 10000 00000000 00000000
    //volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB, -1, 0);
    volatile char *B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    //B = malloc((int)mem_length);
    volatile char *B_off = B + (0x403a40 & 0xFFF);
    printf("%p\n", B);
    printPtr2bin((void*)B);
    

    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {

        unsigned char *text;
        text = (unsigned char *)malloc(AES_BLOCK_SIZE_BYTES);
/*
        for (int i = 0; i < AES_BLOCK_SIZE_BYTES; ++i) {
            text[i] = '\0';
        }
*/

        //for (int i = 0; i < AES_BLOCK_SIZE_BYTES; ++i) {
        //    text[i] = (char)rand()%256;                 //random chars
        //    text[i] = 'A';
        //}

/*
        printf("KEY : ");
        for (int i = 0; i < 32; ++i) {
            printf("%x:", key[i]);
        }
        printf("\nKEY : ");
        for (int i = 0; i < 32; ++i) {
            printf("%x:", key[31-i]);
        }
*/
        u32 enc_key[16];
        printf("ACCESSING\n");
        x += B[2*CACHE_LINE];

        ////rijndaelKeySetupEnc(u32 rk[/*4*(Nr + 1)*/], const u8 cipherKey[], int keyBits);
        //rijndaelKeySetupEnc(enc_key, (const u8*) key, AES_BLOCK_SIZE_BITS);
        ////AES_set_encrypt_key(key, AES_BLOCK_SIZE_BITS, &enc_key);
        ////rijndaelEncrypt(const u32 rk[/*4*(Nr + 1)*/], int Nr, const u8 pt[16], u8 ct[16]);
        //rijndaelEncrypt(enc_key, 10, text, enc_out);
        ////AES_encrypt(text, enc_out, &enc_key);

        //for (int i = 0; B_off+i < B+(int)mem_length; i+=CACHE_LINE) {
          //  x += B_off[i];      // Takes less time for reload without CONNECT
            //B_off[i] = 1;     .. Takes more time for reload without CONNECT
        //}


        //x += B[0+CACHE_LINE];
        //B[0+CACHE_LINE] = x;

/*
        x += B[0];
        x += B[MB(2)];
        x += B[MB(4)];
        x += B[MB(6)];

        for (int j = 0; j < 1; ++j) {
            for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
                B[i+CACHE_LINE] = 1;
            }
        }

        for (int i = 0; i < 1; ++i) {
            for (int i = 0; i < (int)mem_length; i+=CACHE_L3_SET_OFFSET) {
                x = B[i+CACHE_LINE];          // Takes less time for reload without CONNECT    
                //B[i] = i;     // Takes more time for reload without CONNECT
            }    
        }
  */     

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


