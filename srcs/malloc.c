#include "malloc.h"

t_heap	g_heap = {0};

static t_page*	allocate_new_page(t_page* next, size_t page_size)
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

	if (page == MAP_FAILED)
	{
		return NULL;
	}

	page->size = page_size;
	page->next = next;

	block = (t_block *)(page + 1);
	block->size = page_size - sizeof(t_page);
	block->free = 1;

	return page;
}

static void*	enough_space(t_block* block, size_t size)
{
	if (block->size > size)
	{
		t_block*	newBlock;
	
		newBlock = (t_block *)((char *)block + size);
		newBlock->size = block->size - size;
		newBlock->free = 1;
	}

	block->size = size;
	block->free = 0;

	return (char *)block + sizeof(t_block); // Returns the memory shifted by sizeof(t_block) to make the content directly accesible
}

static void*	alloc_in_page(t_page* page, size_t size)
{
	t_block*	block;
	char*		end;

	block	= (t_block *)(page + 1);
	end		= (char *)page + page->size;

	while ((char *)block < end)
	{
		if (block->free && block->size >= size)
		{
			return enough_space(block, size);
		}

		block = (t_block *)((char *)block + block->size); // Jump to next_block
	}

	return NULL;
}

static void*	alloc_in_zone(t_page** zone, size_t size, size_t zoneSize)
{
	t_page*	page;
	void*	ptr;

	page	= *zone;
	ptr		= NULL;
	
	while (page)
	{
		ptr = alloc_in_page(page, size); // Returns NULL if there isnt enough space available

		if (ptr) return ptr;
	
		page = page->next;
	}

	*zone = allocate_new_page(*zone, zoneSize);

	return alloc_in_page(*zone, size);
}

static size_t	align16(size_t size)
{
	return (size + 15) & ~15;
}

void*	malloc(size_t size)
{
	if (size <= 0 || size > __INT_MAX__)
		return NULL;

	static t_page**	zones[] = {
		&g_heap.tiny,
		&g_heap.small,
		&g_heap.large
	};

	size = align16(sizeof(t_block) + size);

	size_t	sizes[] = {
		TINY_ZONE_SIZE,
		SMALL_ZONE_SIZE,
		LARGE_ZONE_SIZE(size)
	};

	int	index = (size > TINY) + (size > SMALL);

	return alloc_in_zone(zones[index], size, sizes[index]);
}

