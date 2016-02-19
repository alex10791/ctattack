// https://www.google.dk/search?client=ubuntu&channel=fs&q=openssl+aes+encryption+ecb&ie=utf-8&oe=utf-8&gws_rd=cr&ei=9ijFVqykKIuuswHBgIXYCA#channel=fs&q=openssl+aes+encryption+example+in+c
/*
 *
 * This server take an aritrary 16 byte input from the TCP stream and encrypt it using the
 * key key[] given below. The server will resond with the encrypted message and close the 
 * connection
 * 
 * Spawn the server like this:
 * ./aes_verbose_server PORT_NUMBER
 * 
 * the server will copy the first 16 bytes into a local array and clean the client message buffer
 * this might affect the cache performance and hence the chache timings
 *
 */


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <openssl/aes.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_TCP_BUFFER_SIZE 2000

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


    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *message, client_message[2000];

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

    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {

        printf("Connection accepted from IP:%s Port:%d\n", inet_ntoa(client.sin_addr), client.sin_port);
        //Reply to the client
        
        recv(new_socket, client_message , SERVER_TCP_BUFFER_SIZE , 0);          
      
        client_message[strlen(client_message) - 1] = '\0';

        
        unsigned char *text;
        text = (unsigned char *)malloc(80);
        unsigned char enc_out[80];
        unsigned char enc_str[240];


        for (int i = 0; i < 80; ++i) {
            text[i] = '\0';
        }

        printf("client_message: %s\n", client_message);
        int len = strlen(client_message);
        for (int i = 0; i < len + 1; ++i) {
            text[i] = (unsigned char)client_message[i];
            client_message[i] = '\0';
        }


        AES_KEY enc_key;

        AES_set_encrypt_key(key, 128, &enc_key);
        AES_encrypt(text, enc_out, &enc_key);


        //message = (char *) malloc( (sizeof("Encrypted Ciphertext: ") + 16 * 3);
        message = (char *) malloc( (sizeof("Encrypted Ciphertext: ")) + strlen((char*)enc_out) * 3);
        sprintf(message, "Encrypted Ciphertext: ");

        enc_str[0] = '\0';
        
        for(int i=0;*(enc_out+i)!=0x00;i++) {
            sprintf((char*)enc_str, "%s%02X:", enc_str, enc_out[i]);
        }

        enc_str[strlen((char*)enc_str) - 1] = '\0';
        
        sprintf(message, "%s%s", message, enc_str);

/*
        printf("String:\t\t");
        for(int i=0;*(client_message+i)!=0x00;i++)
            printf("%c  ",*(client_message+i));
        printf("\noriginal:\t");
        for(int i=0;*(client_message+i)!=0x00;i++)
            printf("%X ",*(client_message+i));
        printf("\nencrypted:\t");
        for(int i=0;*(enc_out+i)!=0x00;i++)
            printf("%X ",*(enc_out+i));
        printf("\n");
*/

        printf("%s\n", message);
        write(new_socket , message , strlen(message));

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

    AES_KEY enc_key, dec_key;

    AES_set_encrypt_key(key, 128, &enc_key);
    AES_encrypt(text, enc_out, &enc_key);      

    AES_set_decrypt_key(key,128,&dec_key);
    AES_decrypt(enc_out, dec_out, &dec_key);

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


