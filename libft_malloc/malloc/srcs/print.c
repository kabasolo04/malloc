#include "malloc.h"

static void	printPage(t_page* page)
{
	t_block*	block;
	char*		end;

	block	= (t_block *)(page + 1);
	end		= (char *)page + page->size;

	while((char *)block < end)
	{
		#if PRINT_ALL_MEM == 1
			if (block->free)
				ft_printf("   - \033[32mFREE[%u]\033[0m\n", block->size);
			else
				ft_printf("   - \033[31mALLO[%u]\033[0m\n", block->size);
		#else
			if (!block->free)
				ft_printf("%x - %x : %u bytes\n", block, block + block->size, block->size);
		#endif

		block = (t_block *)((char *)block + block->size);
	}
}

static void	printZone(t_page* zone, char* zone_name, pthread_mutex_t* mutex)
{
	pthread_mutex_lock(mutex);

	t_page*	page = zone;

	if (!page)
	{
		ft_printf("%s : (empty)\n", zone_name);
		pthread_mutex_unlock(mutex);
		return;
	}

	for (int i = 0; page; i++)
	{
		ft_printf("%s : %x\n", zone_name, page);
		printPage(page);
		page = page->next;
	}

	pthread_mutex_unlock(mutex);
}

void	show_alloc_mem()
{
	printZone(g_heap.tiny, "TINY", &g_heap.tiny_mutex);
	printZone(g_heap.small, "SMALL", &g_heap.small_mutex);
	printZone(g_heap.large, "LARGE", &g_heap.large_mutex);
	ft_printf("\n");
}
