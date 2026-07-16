#include "../includes/malloc.h"

t_page*	allocate_new_page(t_page* next, size_t page_size)
{
	t_page		*page;
	t_block		*block;

	page = mmap(
		NULL,
		page_size,
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1,
		0
	);

	if (page == MAP_FAILED) return NULL;

	page->size = page_size;
	page->next = next;

	block = (t_block *)(page + 1);

	block->size = page_size - sizeof(t_page);
	block->free = 1;

	return (page);
}

void*	enough_space(t_block* block, size_t size)
{
	block->size = block->size - size;

	t_block	*newBlock;

	newBlock = (t_block *)((char *)block + block->size);

	newBlock->size = size;
	newBlock->free = 0;

	return (char *)newBlock + sizeof(t_block);
}

void*	alloc_in_page(t_page* page, size_t size)
{
	t_block*	block;
	t_block*	next_block;
	char*		end;

	block	= (t_block *)(page + 1);
	end		= (char *)page + page->size;

	while ((char *)block < end)
	{
		if (block->free && block->size >= size) return enough_space(block, size);

		next_block = (t_block *)((char *)block + block->size);

		if (block->free && next_block->free) // 2 free blocks next to each other??? lets merge them
		{
			block->size += next_block->size;
			continue;
		}

		block = next_block;
	}

	return NULL;
}

void*	allocate(t_page** zone, size_t size, size_t zoneSize)
{
	t_page*	page = *zone;
	
	while (page)
	{
		void*	ptr = alloc_in_page(page, size); // Returns NULL if there isnt enough space available

		if (ptr) return ptr;
	
		page = page->next;
	}

	*zone = allocate_new_page(*zone, zoneSize);

	return alloc_in_page(*zone, size);
}

size_t align16(size_t size)
{
	return (size + 15) & ~15;
}

void*	malloc(size_t size)
{
	t_page**	zones[] = {
		&g_heap.tiny,
		&g_heap.small,
		&g_heap.large
	};

	size_t	sizes[] = {
		TINY_ZONE_SIZE,
		SMALL_ZONE_SIZE,
		LARGE_ZONE_SIZE(size)
	};

	size = sizeof(t_block) + align16(size);

	char index = (size > TINY) + (size > SMALL);

	return	allocate(zones[index], size, sizes[index]);
}

void	free(void* ptr)
{
	t_block* block;

	block = (t_block*)((char*)ptr - sizeof(t_block));

	block->free = 1;
}

//void*	realloc(void* ptr, size_t size)
//{
//	
//}
//
//void*	calloc(size_t nmemb, size_t size)
//{
//	
//}
