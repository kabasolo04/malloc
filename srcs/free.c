#include "malloc.h"

static void	free_block(t_search* data) // Defragmentation happens here
{
	t_block*	prev;
	t_block*	block;
	char*		end;

	prev	= data->prev_block;
	block	= data->block;
	end		= (char *)data->page + data->page->size;

	if (block->free)
		return error_msg("[Warning]: Double free detected\n");

	block->free = 1;

	t_block *next = (t_block *)((char *)block + block->size);

	if ((char *)next < end && next->free)
		block->size += next->size;

	if (prev && prev->free)
		prev->size += block->size;
}

static void	free_page(t_search* data)
{
	t_page*		prev;
	t_page*		page;
	t_block*	first_block;
	
	prev		= data->prev_page;
	page		= data->page;
	first_block	= (t_block *)(page + 1);

	if (page->size == first_block->size + sizeof(t_block) && first_block->free)
	{
		if (prev)
			prev->next = page->next;
		else
			*data->zone = page->next;

		munmap(page, page->size);
	}
}

void	free(void *ptr)
{
	t_search			data = {0};
	pthread_mutex_t*	mutex;

	mutex = search_block(&data, ptr);

	if (!mutex) return ;

	free_block(&data);
	free_page(&data);

	pthread_mutex_unlock(mutex);
}
