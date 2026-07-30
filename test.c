#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "includes/libft.h"
#include "includes/malloc.h"

#define NUM_ALLOCS 820

int main(void)
{
	show_alloc_mem();

	char *allocs[NUM_ALLOCS];

	for (size_t i = 0; i < NUM_ALLOCS; i++)
		allocs[i] = malloc(60);

	show_alloc_mem();

	/* Grow half of them */
	for (size_t i = 0; i < NUM_ALLOCS; i += 2)
		allocs[i] = realloc(allocs[i], 120);

	show_alloc_mem();

	/* Shrink the other half */
	for (size_t i = 1; i < NUM_ALLOCS; i += 2)
		allocs[i] = realloc(allocs[i], 32);

	show_alloc_mem();

	for (size_t i = 0; i < NUM_ALLOCS; i++)
		free(allocs[i]);

	show_alloc_mem();

	return (0);
}