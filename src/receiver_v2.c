

#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include "ctattack.h"

#define N 20



int register_cache(volatile char* B, int b);


int main(int argc, char* argv[]) {

	int b = CACHE_L3_SIZE;
	volatile char* B;
	unsigned long int tt = 0;
	int start_bit = 0;
	int data = 0;
	int bit_count = 0;
	unsigned int begin, end;
	int x = 0;
	int cache_misses[N];
	int income_bit;

	B = (char*)malloc(b);

	printf("&B\t:\t%x\n", (int)B);

	unsigned long int tt_arr[3];
	int i = 0;

	while(1) {

		income_bit = register_cache(B, b);	
		
		printf("%d\n", (int)income_bit);

		if (income_bit == 1) {
			for (int i = 0; i < N/2; ++i) {
				income_bit = register_cache(B, b);
			}
			start_bit = 1;
			break;
		}	
			
	}



	for (int k = 0; k < 4; ++k) {
		//income_bit = register_cache(B, b);	
		
		//printf("%d\n", (int)income_bit);

		for (int i = 0; i < N; ++i) {
			income_bit = register_cache(B, b);
		}
		printf("%d\n", (int)income_bit);
		data = (data << 1) + income_bit;

	}





	while(1) {
		
		break;
		for (int n = 0; n < N; ++n) {
			tt = 0;
			for (int k = 0; k < ROBOSTNESS_LOOP/N; ++k) {
				begin = timestamp();
				for (int i = 0; i < b; i+=CACHE_LINE) {
					//x += (int)!B[i];
					x += B[i];
				}
				end = timestamp();
				tt += end - begin;
			}
			tt /= ROBOSTNESS_LOOP/N;
			//printf("tt\t:\t%lu\t\t%d\n", tt, (int)(tt > CACHE_L3_THRESHOLD));

			tt_arr[n] = (int)(tt > CACHE_L3_THRESHOLD);
		}
		tt = 0;
		for (int i = 0; i < N; ++i) {
			tt += tt_arr[i];
		}
		if (tt > N/2) {
			tt = CACHE_L3_THRESHOLD + 10;
		} else {
			tt = CACHE_L3_THRESHOLD - 10;
		}
		printf("tt\t:\t%lu\t\t%d\t\tstart_bit\t:\t%d\n", tt, (int)(tt > CACHE_L3_THRESHOLD), start_bit);

		if (start_bit == 1) {
			if (tt < CACHE_L3_THRESHOLD) {
				data = (data << 1) + 1; 
			} else {
				data = data << 1;
			}
			bit_count++;
		}
		if ((start_bit == 0) && (tt > CACHE_L3_THRESHOLD)) {
			start_bit = 1;
		}
		if (bit_count == 4) {
			break;
		}
	}
	
	printf("%d\n", data);

	free((void *)B);

	return 0;
}




int register_cache(volatile char* B, int b) {
	unsigned long int tt = 0;
	int x = 0;
	unsigned int begin, end;

	for (int k = 0; k < ROBOSTNESS_LOOP/N; ++k) {
		begin = timestamp();
		for (int i = 0; i < b; i+=CACHE_LINE) {
			x += B[i];
		}
		end = timestamp();
		tt += end - begin;
	}
	tt /= ROBOSTNESS_LOOP/N;

	return (int)(tt > CACHE_L3_THRESHOLD);
	
}

