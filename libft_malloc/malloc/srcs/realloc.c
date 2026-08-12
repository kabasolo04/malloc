#include "malloc.h"

#define MIN_BLOCK_SIZE (sizeof(t_block) + 16)

static char zone_change_required(size_t old_size, size_t new_size)
{
	char	old_zone;
	char	new_zone;

	old_size -= sizeof(t_block);

	old_zone = (old_size > TINY) + (old_size > SMALL);
	new_zone = (new_size > TINY) + (new_size > SMALL);

	return (old_zone != new_zone || old_zone == 2 || new_zone == 2);
}

static char expand(t_search *data, size_t size)
{
	t_block*	block;
	t_block*	next_block;
	t_block*	new_block;
	size_t		growth;
	size_t		remainder;
	char*		end;

	block		= data->block;
	next_block	= (t_block *)((char *)block + block->size);
	end			= (char *)data->page + data->page->size;
	growth		= size - block->size;

	if ((char *)next_block + sizeof(t_block) > end)	return (0);
	if (!next_block->free)							return (0);
	if (next_block->size < growth)					return (0);

	remainder = next_block->size - growth;

	if (remainder < MIN_BLOCK_SIZE)
	{
		block->size += next_block->size;
		return (1);
	}

	new_block = (t_block *)((char *)block + size);
	new_block->size = remainder;
	new_block->free = 1;

	block->size = size;
	
	return (1);
}

static void shrink(t_search *data, size_t size)
{
	t_block*	block;
	t_block*	next_block;
	t_block*	new_block;
	size_t		remainder;
	char*		end;
	
	block		= data->block;
	next_block	= (t_block *)((char *)block + block->size);
	new_block	= (t_block *)((char *)block + size);
	end			= (char *)data->page + data->page->size;
	remainder	= block->size - size;
	
	if (remainder < MIN_BLOCK_SIZE) return;

	new_block->size = remainder;
	new_block->free = 1;
	
	block->size = size;

	if ((char *)next_block + sizeof(t_block) <= end && next_block->free)	// Defragmentation
		new_block->size += next_block->size;
}

static char resize(t_search *data, size_t size)
{
	if (zone_change_required(data->block->size, size))
		return (0);

	size = align16(size + sizeof(t_block));

	if (data->block->size < size)
		return (expand(data, size));

	if (data->block->size > size)
		shrink(data, size);

	return (1);
}

static void*	new_alloc(void *ptr, size_t old_size, size_t new_size)
{
	void*	new_ptr = malloc(new_size);

	if (!new_ptr) return NULL;

	ft_memcpy(new_ptr, ptr, ft_min(old_size, new_size));

	free(ptr);

	return new_ptr;
}

void*	realloc(void *ptr, size_t size)
{
	t_search			data = {0};
	pthread_mutex_t*	mutex;
	char				resize_worked;
	size_t				old_size;

	if (!ptr)				return malloc(size);
	if (size == 0)			return free(ptr), NULL;
	if (size > __INT_MAX__)	return NULL;

	mutex = search_block(&data, ptr);	// Mutex lock inside search_block!

	if (!mutex) return NULL;

	resize_worked = resize(&data, size);

	old_size = data.block->size - sizeof(t_block);

	pthread_mutex_unlock(mutex);

	if (resize_worked) return ptr;

	return new_alloc(ptr, old_size, size);
}
