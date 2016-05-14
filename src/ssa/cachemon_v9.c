#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ctattack.h"

#include <math.h>

#define RAM_ACCESS_TIME_EMPIRICAL 299
#define CACHE_LINE_DISTANCES 16
#define CACHE_LINE_CHECK_OFFSET (0x3c47c0640 & 0x00003FFFF)       //0x3b3a55840
#define CACHE_LINE_VICTIM_OFFSET 256    //(0x39e987880 & 0x00003FFFF)
#define REPS 1000000

int main(int argc, char* argv[])
{

    unsigned long int tt = 0, tt2 = 0;
    unsigned long int begin, end;
    unsigned long int begin2, end2;
    unsigned long int x = 0;

    int times[REPS];

    volatile char **tmp1;
//    volatile char **tmp2;
//    volatile char **tmp3;
    
//    char ch = '\0';
    FILE *fp;

    size_t mem_length = (size_t)MB(2); 
    int mem_length_char = ((int)mem_length/sizeof(char));
    int mem_length_ptr = (int)mem_length/sizeof(void *);


    volatile char **B = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char **C = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    volatile char *D = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);


    // brings some data in Exclusive state (OR Shared state - probably exclusive)
    x = B[MB(0)];
    x = C[MB(0)];
    x = D[MB(0)];

    // This is used to render the stride prefetcher useless... hopefully

    // This is used to index into the same cache slice.
    volatile char *B_arr[24];
 /*  
    B_arr[0]  = B + (0x00 << 18);
    B_arr[1]  = B + (0x01 << 18);
    B_arr[2]  = B + (0x02 << 18);
    B_arr[3]  = B + (0x03 << 18);
    B_arr[4]  = B + (0x04 << 18);
    B_arr[5]  = B + (0x05 << 18);
    B_arr[6]  = B + (0x06 << 18);
    B_arr[7]  = B + (0x07 << 18);

    B_arr[8]  = C + (0x00 << 18);
    B_arr[9]  = C + (0x01 << 18);
    B_arr[10] = C + (0x02 << 18);
    B_arr[11] = C + (0x03 << 18);
    B_arr[12] = C + (0x04 << 18);
    B_arr[13] = C + (0x05 << 18);
    B_arr[14] = C + (0x06 << 18);
    B_arr[15] = C + (0x07 << 18);
    
    for (int i = 0; i < 16; ++i) {
        printPtr2bin((void *)B_arr[i]);
    }
    
    printf("\n");
*/

    B[(0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    B[(0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    B[(0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    B[(0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    B[(0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    B[(0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    B[(0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    B[(0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);

    C[(0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    C[(0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    C[(0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    C[(0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    C[(0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    C[(0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    C[(0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(C + (0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);
    C[(0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8] = (volatile char *)(B + (0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8);

    B[(0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    B[(0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    B[(0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    B[(0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    B[(0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    B[(0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    B[(0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    B[(0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);

    C[(0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    C[(0x01 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    C[(0x02 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    C[(0x03 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    C[(0x04 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    C[(0x05 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    C[(0x06 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(C + (0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);
    C[(0x07 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8] = (volatile char *)(B + (0x00 << 18)/8 + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8);


for (int i = 0; i < REPS; ++i) {
    times[i] = 0;
}

int count = 0;

tt2 = 0;
//while (tt2 > 3000) {
for (count = 0; count < REPS; ++count) {

    if ((argc != 3) && (argc != 4)) {
        printf("[!]Usage: %s IP PORT [OUTFILE]\n", argv[0]);
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
    //printf("socket_desc: %d\n", socket_desc);
    server.sin_addr.s_addr = inet_addr(ip_str);     //80.239.174.120
    server.sin_family = AF_INET;
    server.sin_port = htons( server_port );
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("connection error\n");
        return 1;
    }
    //printf("Connected\n");



    //count++;


    TIMESTAMP_START;
    TIMESTAMP_STOP;
    TIMESTAMP_START;
    TIMESTAMP_STOP;


    // PRIME
    tmp1 = B + CACHE_LINE_CHECK_OFFSET/8;
    for (int i = 0; i < 16; ++i) {
        //tmp1 = B_arr[i]+CACHE_LINE_CHECK_OFFSET;
        //x += *tmp1;
        tmp1 = *tmp1;
    }

    // REPRIME
    // Second Load to clear L1 and L2 caches
    tmp1 = B + CACHE_LINE_CHECK_OFFSET/8 + KB(32)/8;
    for (int i = 0; i < 16; ++i) {
        //tmp1 = B_arr[i]+CACHE_LINE_CHECK_OFFSET+KB(32);
        //x += *tmp1;
        tmp1 = *tmp1;
    }

    //printf("\n");


    //message = "GET / HTTP/1.1\r\n\r\n";
    message = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    message = "aaaabbbbccccdddd";
    if( send(socket_desc , message , 16 , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    //puts("Data Send\n");
    if( recv(socket_desc, server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }

    //for (int i = 0; i < 16; ++i) {
    //    printf("%02x", server_reply[i]);
    //}
    //printf("\n");

    // VICTIM RUN
//    x += *(D+CACHE_LINE_VICTIM_OFFSET);
    //x += *(D + (1 << 17));


    // PROBE & MEASURE
    //tmp1 = B;
    //tt2 = 0;
    tmp1 = B + CACHE_LINE_CHECK_OFFSET/8;
    TIMESTAMP_START;
    for (int i = 0; i < 16; ++i) {
        //tmp1 = B_arr[i]+CACHE_LINE_CHECK_OFFSET;
        //TIMESTAMP_START;
        //x += *tmp1;
        tmp1 = *tmp1;
        //x += B[i];
/*     
        TIMESTAMP_STOP;
        begin = get_global_timestamp_start();
        end = get_global_timestamp_stop();
        //TIMESTAMP_START;
        //TIMESTAMP_STOP;
        //begin2 = get_global_timestamp_start();
        //end2 = get_global_timestamp_stop();
        tt2 += (end-begin);//-(end2-begin2);
*/    
        //printf("%p: %lu\n", tmp1, (end-begin)-(end2-begin2) );
    }
   
    TIMESTAMP_STOP;
    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();
    TIMESTAMP_START;
    TIMESTAMP_STOP;
    begin2 = get_global_timestamp_start();
    end2 = get_global_timestamp_stop();
    tt2 += (end-begin);//-(end2-begin2);

    times[count] = (end-begin);

    //puts("Reply received\n");
    //puts(server_reply);
    close(socket_desc);

/*
    for (int i = 0; i < 16; ++i) {
        printf("%02X::", (unsigned char)server_reply[i]);
    }
    printf("\n");
*/
    //printf("%s\n", server_reply);

}

    printf(" tt: %lu\n", tt);
    printf("tt2: %lu\n", tt2/REPS);
    printf("count: %d\n", count);

    printf("\n");

    TIMESTAMP_START;
    TIMESTAMP_STOP;


    begin = get_global_timestamp_start();
    end = get_global_timestamp_stop();


    printf("%lu\n", end-begin);
















// STATISTICS

    int Number = REPS;
    float Mean, Variance, SD, Sum=0, Differ, Varsum=0;
    int min=999999, max=0;


    for(int i=0; i<Number; i++)
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
 
    for(int i=0; i<Number; i++)
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




// EXPORTING


    char dir[256];

    if (argc == 3) {
        sprintf(dir, "%s", "data.csv");
    } else {
        sprintf(dir, "%s", argv[3]);
    }
    

    fp = fopen((const char*)dir, "w");

    for (int i = 0; i < REPS; ++i) {
        fprintf(fp, "%d\n", times[i]);
    }

    fclose(fp);




    return 0;
}



