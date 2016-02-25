

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "ctattack.h"

#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)

/*
 *
 *	void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
 *  int munmap(void *addr, size_t length);
 *
 *
 *	size_t mem_length;
 *	char (*my_memory)[sysconf(_SC_PAGESIZE)] = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
 *
 */


int main(int argc, char* argv[]) {

	//void* kokos = mmap(void *addr, size_t length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_HUGE_2MB, int fd, (off_t)0);

	size_t mem_length = (size_t)2*1024*1024; // 10000 00000000 00000000
	//char *my_memory = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
	//char *my_memory = mmap((void*)0x200000000000, mem_length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS | MAP_HUGE_2MB, -1, 0);
	char *my_memory = mmap(NULL, mem_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB, -1, 0);

	my_memory[2*1024*1024-1] = 123;

	printf("0x%x\n", MAP_HUGE_2MB);
	printf("0x%x\n", MAP_HUGE_SHIFT);

	printf("%lu\n", sizeof(*my_memory));
	printf("%p\n", my_memory);

	while (1);

	return 0;
}



// note
// kenneho.net/2011/03/12/experimenting-with-hugepages-to-reduce-cpu-and-memory-resources/
