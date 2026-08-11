#include "libft.h"

int	ft_abs(int n)
{
	return n * (1 - 2 * (n < 0));
}
