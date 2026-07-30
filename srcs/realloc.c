#include "malloc.h"

#define MIN_BLOCK_SIZE (sizeof(t_block) + 16)

void*	shrink_block(t_search* data, void *ptr, size_t size)
{
	t_block*	block;
	char*		end;

	block	= data->block;
	end		= (char *)data->page + data->page->size;

	size_t	newSize = block->size - size;

	t_block*	new_next	= (t_block *)((char *)block + size);
	t_block*	next		= (t_block *)((char *)block + block->size);

	if (newSize < MIN_BLOCK_SIZE || (char*)new_next >= end)
		return ptr;

	if ((char*)next < end && next->free)
		newSize += next->size;

	block->size		= size;
	new_next->free	= 1;
	new_next->size	= newSize;

	return ptr;
}

void*	expand_block(t_search* data, void *ptr, size_t size)
{
	t_block*	block;
	char*		end;

	block	= data->block;
	end		= (char *)data->page + data->page->size;

	t_block*	new_next	= (t_block *)((char *)block + size);
	t_block*	next		= (t_block *)((char *)block + block->size);

	if ((char*)new_next >= end || !next->free || next->size < size - block->size + MIN_BLOCK_SIZE)
	{
		void*	new_ptr = malloc(size);

		if (!new_ptr)
			return NULL;

		ft_memcpy(new_ptr, ptr, size);

		free_block(data);
		free_page(data);

		return new_ptr;
	}

	new_next->size = next->size + block->size - size;
	new_next->free = 1;

	block->size = size;

	return ptr;
}

void*	realloc(void *ptr, size_t size)
{
	if (!ptr)
		return malloc(size);
	if (!size)
		return free(ptr), NULL;

	t_search data = {0};

	if (!search_block(&data, ptr))
		return error_msg("[Warning]: Memory address not found\n"), NULL;

	if (data.block->size == size)
		return ptr;

	if (data.block->size > size)
		return shrink_block(&data, ptr, size);
	else
		return expand_block(&data, ptr, size);
}
