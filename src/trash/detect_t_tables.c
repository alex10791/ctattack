

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "ctattack.h"


int main(int argc, char* argv[]) {


	int b = CACHE_L3_SIZE;
	volatile char* B;
	unsigned long int tt = 0;
	unsigned int begin, end;
	int x = 0;

	//volatile char* B_ext;
	//char *B_ext = (char*)mmap(NULL, 2*mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	// //B_ext = (char*)malloc(2*b);
	//B = (char *) (((unsigned long int)B_ext & (~(unsigned long int)CACHE_L3_SIZE_MASK)) + CACHE_L3_SIZE);
	size_t mem_length = MB(3);
	B = (char*)mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	
	//B[MB(2)-1] = 0x23;
	//printf("%d\n", B[MB(2)-1]);

	//printf("B_ext\t%p\t\n", B_ext);
	//printPtr2bin((void*)B_ext);
	printf("B\t%p\t\n", B);
	printPtr2bin((void*)B);



	while (1) {
		for (int i = 0; i < MB(3)-1; ++i) {
			begin = timestamp();
			B[i] = 0x01;
			end = timestamp();
			tt = end - begin;
		}
	}







	return 0;
}



// note
// kenneho.net/2011/03/12/experimenting-with-hugepages-to-reduce-cpu-and-memory-resources/
