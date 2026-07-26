#include "malloc.h"

void	printPage(t_page* page)
{
	t_block*	block;
	char*		end;

	block	= (t_block *)(page + 1);
	end		= (char *)page + page->size;

	while((char *)block < end)
	{
		if (block->free)
			ft_printf("   - \033[32mFREE[%u]\033[0m\n", block->size);
		else
			ft_printf("   - \033[31mALLO[%u]\033[0m\n", block->size);

		block = (t_block *)((char *)block + block->size);
	}
}

void	printZone(t_page* zone)
{
	t_page*	page = zone;

	if (!page)
	{
		ft_printf("  > EMPTY\n\n");
		return;
	}

	for (int i = 0; page; i++)
	{
		ft_printf("  > PAGE_ID[%d]\n", i);
		printPage(page);
		page = page->next;
	}

	ft_printf("\n");
}

void	show_alloc_mem()
{
	ft_printf("\n");
	ft_printf(" MEM_ZONE[TINY]\n");
	printZone(g_heap.tiny);
	ft_printf(" MEM_ZONE[SMALL]\n");
	printZone(g_heap.small); 
	ft_printf(" MEM_ZONE[LARGE]\n");
	printZone(g_heap.large);
	ft_printf("-----------------------------\n");
}
