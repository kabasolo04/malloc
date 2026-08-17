#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREADS 20
#define ITERATIONS 10000

static void *worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++)
    {
        size_t size = (i % 256) + 1;
        char *ptr = malloc(size);

        if (!ptr)
        {
            printf("malloc failed\n");
            return NULL;
        }

        memset(ptr, 'A', size);

        for (size_t j = 0; j < size; j++)
        {
            if (ptr[j] != 'A')
            {
                printf("MEMORY CORRUPTION!\n");
                return NULL;
            }
        }

        free(ptr);
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREADS];

    for (int i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("done\n");
    return 0;
}