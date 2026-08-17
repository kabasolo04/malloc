#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

int main()
{
	char* ptr = malloc(sizeof(char) * 5);

	show_alloc_mem();

	ft_memcpy(ptr, "HELLO", 300);

	ptr[65503] = 'h';	// **A**

//	ptr[65504] = 'h';	// **B**

	printf("%s\n", ptr);

	free(ptr);

	return 0;
}

/*

 A) This does NOT create a segfault since the memory next to our allocation is also alloct by us

 B) This one does create a segfault since it tries to go 1 byte further than the allocated page size

*/