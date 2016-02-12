

#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include "ctattack.h"

#define STALL_TIME 5

int main(int argc, char* argv[]) {

	int b = CACHE_L3_SIZE;
	volatile char* B;
	//int x = 0;
	//int tt = 0;

	volatile char* B_ext;
	//B = (char*)malloc(b);
	B_ext = (char*)malloc(2*b);
	B = (char *) (((unsigned long int)B_ext & (~(unsigned long int)CACHE_L3_SIZE_MASK)) + CACHE_L3_SIZE);

	printf("ROBOSTNESS_LOOP : %d\n", (int)ROBOSTNESS_LOOP);

	while(1) {
		for (int i = 0; i < b; i+=CACHE_L3_SET_OFFSET) {
			B[i+CACHE_LINE] = 1;
		}
	}

	printf("Sending 1\n");
	for (int k = 0; k < ROBOSTNESS_LOOP; ++k) {
		for (int i = 0; i < b; i+=CACHE_L3_SET_OFFSET) {
			B[i] = 1;
		}
	}

	printf("Sending 1\n");
	for (int k = 0; k < ROBOSTNESS_LOOP; ++k) {
		for (int i = 0; i < b; i+=CACHE_L3_SET_OFFSET) {
			B[i] = 1;
		}
	}

	printf("Sending 0\n");
	for (int k = 0; k < ROBOSTNESS_LOOP; ++k) {
		for (int i = 0; i < b; i+=CACHE_L3_SET_OFFSET) {
			//B[i] = 0;
			for(int j = 0; j < STALL_TIME; ++j);
		}
	}

	printf("Sending 0\n");
	for (int k = 0; k < ROBOSTNESS_LOOP; ++k) {
		for (int i = 0; i < b; i+=CACHE_L3_SET_OFFSET) {
			//B[i] = 1;
			for(int j = 0; j < STALL_TIME; ++j);
		}
	}

	printf("Sending 1\n");
	for (int k = 0; k < ROBOSTNESS_LOOP; ++k) {
		for (int i = 0; i < b; i+=CACHE_L3_SET_OFFSET) {
			B[i] = 1;
		}
	}

/*
	for (int k = 0; k < ROBOSTNESS_LOOP; ++k) {
		for (int i = 0; i < b; i+=CACHE_LINE) {
			B[i] = 1;
		}
	}
*/	

	



	free((void *)B_ext);

	return 0;
}