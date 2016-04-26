

#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include "ctattack.h"



int main(int argc, char* argv[]) {
	unsigned int begin, end, exec_time;
	int b = CACHE_L1_SIZE;
	volatile char* B;
	volatile char* B2;
	//char* B;
	int x = 0;
	unsigned long int tt = 0;

	//printf("%lu\n", sizeof(char));

	if (argc != 4) {
		printf("[!] Usage: %s [CACHE] [MULT] [DIV]\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (atoi(argv[3]) == 0) {
		printf("Divisor should not be 0\n");
		return EXIT_FAILURE;
	}

	if (atoi(argv[1]) == 1) {
		b = CACHE_L1_SIZE;
	} else if (atoi(argv[1]) == 2) {
		b = CACHE_L2_SIZE;
	} else if (atoi(argv[1]) == 3) {
		b = CACHE_L3_SIZE;
	}

	b = atoi(argv[2]) * b / atoi(argv[3]);


	for (int k = 0; k < 100; ++k) {
		// allocate buffer memory
		B = (char*)malloc(b);
		B2 = (char*)malloc(b);

		//for (int j = 0; j < 100; ++j) {
			for (int i = 0; i < b; i+=CACHE_LINE) {
				//x += (int)!B[i];
				B[i] = 1;
			}
		//}

		//for (int j = 0; j < 100; ++j) {
			for (int i = 0; i < b; i+=CACHE_LINE) {
				//x += (int)!B[i];
				//B2[i] = 2;
			}
		//}
		

		// set clock counter
		begin = timestamp();
		
		// normal execution
		//cout << "kokos" << endl;

		//for (int j = 0; j < 10; ++j) {
			for (int i = 0; i < b; i+=CACHE_LINE) {
				//x += (int)!B[i];
				x += B[i];
			}
		//}

		// end clock counter
		end = timestamp();


		free((void *)B);
		free((void *)B2);
		//free(B);
		
		tt += end - begin;

	}



	printf("x\t:\t%d\n", x);

	// estimation and output of execution time
	exec_time = end - begin;
	printf("exec_time\t:\t%d\n", tt/100);
	//cout << "exec_time\t:\t" << exec_time << endl;




	return 0;
}


