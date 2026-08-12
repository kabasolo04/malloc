#include "malloc.h"

void*	calloc(size_t n, size_t size)
{
	void*	ptr;

	ptr = malloc(n * size);

	if (!ptr)
		return NULL;

	return ft_memset(ptr, 0, n * size);
}
