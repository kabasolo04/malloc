
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define THREADS       50
#define ITERATIONS    10000
#define SLOTS         64

typedef struct s_test
{
	void	*ptr[SLOTS];
	size_t	size[SLOTS];
	unsigned int seed;
	int		id;
}	t_test;

static size_t	random_size(unsigned int *seed)
{
	static const size_t sizes[] = {
		1, 2, 7, 8, 15, 16,
		24, 31, 32, 48, 64,
		100, 128, 256, 512,
		1024, 2048, 4096,
		8192, 16384, 65536
	};

	return (sizes[rand_r(seed) % (sizeof(sizes) / sizeof(sizes[0]))]);
}

static void	fill_block(void *ptr, size_t size, unsigned char value)
{
	if (ptr)
		memset(ptr, value, size);
}

static int	check_block(void *ptr, size_t size, unsigned char value)
{
	unsigned char	*p;
	size_t			i;

	if (!ptr)
		return (1);

	p = ptr;
	for (i = 0; i < size; i++)
	{
		if (p[i] != value)
			return (0);
	}
	return (1);
}

static void	*worker(void *arg)
{
	t_test		*test;
	unsigned int	r;
	int			i;
	int			slot;
	size_t		new_size;
	void		*tmp;

	test = arg;

	for (i = 0; i < ITERATIONS; i++)
	{
		r = rand_r(&test->seed);
		slot = r % SLOTS;

		if (!test->ptr[slot])
		{
			test->size[slot] = random_size(&test->seed);
			test->ptr[slot] = malloc(test->size[slot]);

			if (!test->ptr[slot])
			{
				fprintf(stderr, "THREAD %d: malloc failed\n", test->id);
				continue;
			}

			fill_block(
				test->ptr[slot],
				test->size[slot],
				(unsigned char)test->id
			);
		}
		else if (r % 3 == 0)
		{
			if (!check_block(
					test->ptr[slot],
					test->size[slot],
					(unsigned char)test->id))
			{
				fprintf(
					stderr,
					"THREAD %d: MEMORY CORRUPTION BEFORE REALLOC\n",
					test->id
				);
				exit(EXIT_FAILURE);
			}

			new_size = random_size(&test->seed);
			tmp = realloc(test->ptr[slot], new_size);

			if (!tmp)
				continue;

			test->ptr[slot] = tmp;
			test->size[slot] = new_size;

			fill_block(
				test->ptr[slot],
				test->size[slot],
				(unsigned char)test->id
			);
		}
		else if (r % 3 == 1)
		{
			if (!check_block(
					test->ptr[slot],
					test->size[slot],
					(unsigned char)test->id))
			{
				fprintf(
					stderr,
					"THREAD %d: MEMORY CORRUPTION\n",
					test->id
				);
				exit(EXIT_FAILURE);
			}

			free(test->ptr[slot]);
			test->ptr[slot] = NULL;
			test->size[slot] = 0;
		}
		else
		{
			if (!check_block(
					test->ptr[slot],
					test->size[slot],
					(unsigned char)test->id))
			{
				fprintf(
					stderr,
					"THREAD %d: MEMORY CORRUPTION\n",
					test->id
				);
				exit(EXIT_FAILURE);
			}
		}
	}

	for (i = 0; i < SLOTS; i++)
	{
		if (test->ptr[i])
		{
			if (!check_block(
					test->ptr[i],
					test->size[i],
					(unsigned char)test->id))
			{
				fprintf(
					stderr,
					"THREAD %d: FINAL MEMORY CORRUPTION\n",
					test->id
				);
				exit(EXIT_FAILURE);
			}

			free(test->ptr[i]);
		}
	}

	return (NULL);
}

int	main(void)
{
	pthread_t	threads[THREADS];
	t_test		tests[THREADS];
	int			i;

	printf("========================================\n");
	printf("     MALLOC THREAD SAFETY STRESS TEST\n");
	printf("========================================\n");
	printf("Threads:    %d\n", THREADS);
	printf("Iterations: %d per thread\n", ITERATIONS);
	printf("Slots:      %d per thread\n", SLOTS);
	printf("----------------------------------------\n");

	for (i = 0; i < THREADS; i++)
	{
		memset(&tests[i], 0, sizeof(t_test));
		tests[i].id = i + 1;
		tests[i].seed = 0x12345678u + (unsigned int)i * 7919u;

		if (pthread_create(&threads[i], NULL, worker, &tests[i]) != 0)
		{
			fprintf(stderr, "Failed to create thread %d\n", i);
			return (1);
		}
	}

	for (i = 0; i < THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("----------------------------------------\n");
	printf("ALL THREADS FINISHED SUCCESSFULLY\n");
	printf("No memory corruption detected.\n");
	printf("========================================\n");

	return (0);
}
