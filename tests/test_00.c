#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

int main(void)
{
	void	*a;
	void	*b;
	void	*c;

	a = malloc(1);
	b = malloc(10);
	c = malloc(100);

	show_alloc_mem();

	free(a);
	free(b);
	free(c);

	return (0);
}
