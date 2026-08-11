#include "malloc.h"

static int	search_in_page_for_block(t_search* data, t_page* page, void *ptr)
{
	t_block*	prev;
	t_block*	block;
	char*		end;

	prev	= NULL;
	block	= (t_block *)(page + 1);
	end		= (char *)page + page->size;

	while ((char *)block < end)
	{
		if ((char *)block + sizeof(t_block) == (char *)ptr)
		{
			data->prev_block	= prev;
			data->block			= block;
	
			return 1;
		}

		prev	= block;
		block	= (t_block *)((char *)block + block->size);
	}

	return 0;
}

static int	search_in_zone_for_page(t_search* data, void *ptr)
{
	t_page*		prev;
	t_page*		page;

	prev = NULL;
	page = *data->zone;

	while (page)
	{
		if (search_in_page_for_block(data, page, ptr))
		{
			data->prev_page	= prev;
			data->page		= page;
		
			return 1;
		}

		prev = page;
		page = page->next;
	}

	return 0;
}

t_page**	zones[] =
{
	&g_heap.tiny,
	&g_heap.small,
	&g_heap.large,
	NULL
};

static pthread_mutex_t*	mutexes[] =
{
	&g_heap.tiny_mutex,
	&g_heap.small_mutex,
	&g_heap.large_mutex,
	NULL
};

pthread_mutex_t*	search_block(t_search* data, void *ptr)	// Fills the data struct and returns a locked mutex of the zone the ptr belongs to, esle returns NULL
{
	if (!ptr) return NULL;

	for (size_t i = 0; zones[i]; i++)
	{
		pthread_mutex_lock(mutexes[i]);

		data->zone = zones[i];

		if (search_in_zone_for_page(data, ptr))
			return mutexes[i];

		pthread_mutex_unlock(mutexes[i]);
	}
	
	return error_msg("[Warning]: Memory address not found\n"), NULL;
}

void	error_msg(char* msg)
{
	if (!msg) return;

	write(2, msg, ft_strlen(msg));
}

size_t	align16(size_t size) // It simply makes the number bigger so its divisible by 16 unless it already is
{
	return (size + 15) & ~15;
}
