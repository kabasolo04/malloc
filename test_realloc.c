#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

#define THREADS 4
#define ITERS 2000

static void *worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < ITERS; i++)
    {
        void *p = malloc(64);
        if (p)
            p = realloc(p, 128 + (i % 32));
        if (p)
            free(p);

        sleep(0.1);
    }
    return NULL;
}

int main(void)
{
    pthread_t th[THREADS];
    for (int i = 0; i < THREADS; i++)
        pthread_create(&th[i], NULL, worker, NULL);

    show_alloc_mem();
    
    for (int i = 0; i < THREADS; i++)
        pthread_join(th[i], NULL);
    puts("realloc threads test passed");
    return 0;
}
