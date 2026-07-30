#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <stddef.h>
#include "libft.h"

# define TINY 128
# define SMALL 1024

# define TINY_ZONE_SIZE			(16 * getpagesize())
# define SMALL_ZONE_SIZE		(128 * getpagesize())
# define LARGE_ZONE_SIZE(size)	(align16(sizeof(t_page) + (size)))

typedef struct s_block
{
	size_t	size;
	char	free;

} t_block;

typedef struct s_page
{
	size_t			size;
	struct s_page*	next;

} t_page;

typedef struct s_heap
{
	t_page	*tiny;
	t_page	*small;
	t_page	*large;

} t_heap;

typedef struct s_search
{
	t_page**	zone;

	t_page*		prev_page;
	t_page*		page;

	t_block*	prev_block;
	t_block*	block;

} t_search;

extern t_heap g_heap;

void	show_alloc_mem();
int		search_block(t_search* data, void *ptr);
void	error_msg(char* msg);
void	free_block(t_search* data);
void	free_page(t_search* data);

#endif