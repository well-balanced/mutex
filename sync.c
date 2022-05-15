#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define THREAD_NUM 3

pthread_t tid[3];
int counter = 0;
pthread_mutex_t lock;

void *doSomething(void *arg)
{
    char *t_name = (char *)arg;

    pthread_mutex_lock(&lock);
    printf("\n%s started! counter=%d\n", t_name, ++counter);

    srand((unsigned long)time(NULL));
    unsigned long max_iter = 0xFFFFFFFF - (rand() % 0xFFFFFFFF);

    for (unsigned long i = 0; i < max_iter; i++)
        ;

    printf("\n%s finished! counter=%d\n", t_name, --counter);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void)
{
    int i = 0;
    int err;
    char strbuf[THREAD_NUM][8];

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutext init failed\n");
        return 1;
    }

    for (i = 0; i < THREAD_NUM; i++)
    {
        sprintf(strbuf[i], "[T%d", i);
        err = pthread_create(&(tid[i]), NULL, &doSomething, strbuf[i]);
        if (err != 0)
        {
            printf("\ncan't create thread: [%s]", strerror(err));
        }
    }

    for (i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    printf("\n");
    return 0;
}