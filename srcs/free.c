#include "malloc.h"

int	free_block(t_block *prev, t_block *curr, char *end) // Defragmentation happens here
{
	if (curr->free)
	{
		ft_printf("[Warning]: Double free\n");
		return 1;
	}

	curr->free = 1;

	t_block *next = (t_block *)((char *)curr + curr->size);

	if ((char *)next < end && next->free)
		curr->size += next->size;

	if (prev && prev->free)
		prev->size += curr->size;

	return 1;
}

int	search_in_page_for_block(t_page* page, t_block* target_block)
{
	t_block*	prev;
	t_block*	curr;
	char*		end;

	prev	= NULL;
	curr	= (t_block *)(page + 1);
	end		= (char *)page + page->size;

	while ((char *)curr < end)
	{
		if (curr == target_block)
			return free_block(prev, curr, end);

		prev = curr;
		curr = (t_block *)((char *)curr + curr->size);
	}

	return 0;
}

int	free_page(t_page *prev, t_page *curr, t_page** zone)
{
	t_block*	first_block;
	
	first_block	= (t_block *)(curr + 1);

	if (curr->size == first_block->size + sizeof(t_block) && first_block->free)
	{
		if (prev)
			prev->next = curr->next;
		else
			*zone = curr->next;

		munmap(curr, curr->size);
	}

	return 1;
}

int	search_in_zone_for_page(t_page** zone, t_block* block)
{
	t_page*	prev;
	t_page*	curr;

	prev	= NULL;
	curr	= *zone;

	while (curr)
	{
		if (search_in_page_for_block(curr, block))
			return free_page(prev, curr, zone);

		prev = curr;
		curr = curr->next;
	}

	return 0;
}

void	free(void *ptr)
{
	if (!ptr) return;

	t_block*	block = (t_block *)((char *)ptr - sizeof(t_block));
	
	t_page**	zones[] = {
		&g_heap.tiny,
		&g_heap.small,
		&g_heap.large,
		NULL
	};

	for (size_t i = 0; zones[i]; i++)
	{
		if (search_in_zone_for_page(zones[i], block))
			return ;
	}

	ft_printf("[Warning]: Not my memory\n");
}
