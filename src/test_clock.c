

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CACHE_LINE 64
#define CACHE_L1_SIZE 32768				// Data Cache
#define CACHE_L2_SIZE 262144
#define CACHE_L3_SIZE 3145728
#define CACHE_L2_ASSOCIATIVITY 8

int main(int argc, char* argv[]) {
	// declaration
	clock_t begin, end, exec_time;
	int b = CACHE_L1_SIZE * 4;
	char* B = (char*)malloc(b);
	int x = 0;

	// set clock counter
	begin = clock();

	// normal execution
	//cout << "kokos" << endl;

	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < b; i+=CACHE_LINE) {
			x += (int)!B[i];
		}
	}

	// end clock counter
	end = clock();


	printf("x\t:\t%d\n", x);

	// estimation and output of execution time
	exec_time = end - begin;
	printf("exec_time\t:\t%ld\n", (long int)exec_time);
	//cout << "exec_time\t:\t" << exec_time << endl;

	return 0;
}

