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


        
    char *text;
    unsigned char enc_out[80];
    unsigned char enc_str[240];


    for (int i = 0; i < 80; ++i) {
        enc_out[i] = '\0';
    }

    enc_str[0] = '\0';


    for (int i = 0; i < 16; ++i) {
        text = message_arr[i];
        for (int j = 0; j < 16; ++j) {
            printf("%02x:", text[j]);
        }
        printf("\n");

        u32 enc_key[32];
        rijndaelKeySetupEnc((u32*)enc_key, (const u8*) key, (int)AES_BLOCK_SIZE_BITS);
    }


    printf("------------------------------------------------\n");
    for (int i = 0; i < 16; ++i) {

        u32 enc_key[16];
        printf("i : %d\n", i);
        text = message_arr[i];

        printf("%p\n", text);
        for (int j = 0; j < 16; ++j) {
            printf("%02x:", text[i]);
        }
        printf("\n");

        rijndaelKeySetupEnc((u32*)enc_key, (const u8*) key, (int)AES_BLOCK_SIZE_BITS);
        rijndaelEncrypt((const u32*)enc_key, (int)10, (const u8*)text, (u8*)enc_out);
    }




    return 0;
}


