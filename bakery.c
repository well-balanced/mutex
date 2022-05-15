#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define THREAD_COUNT 32

volatile int numbers[THREAD_COUNT] = {0};
volatile int choosing[THREAD_COUNT] = {0};
volatile int counter = 0;

void lock(int);
void unlock(int);
void use_resource(int);
void *thread_body(void *);

int main(int argc, char **argv)
{
    pthread_t threads[THREAD_COUNT];

    srand(time(NULL));

    printf("\n number    thread_id    counter\n");
    fflush(stdout);
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        pthread_create(&threads[i], NULL, &thread_body, (void *)((long)i));
    }

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    printf("\n");
    return 0;
}

void *thread_body(void *arg)
{
    long t_id = (long)arg;

    usleep(rand() % 0xFFFFFF);
    lock(t_id);

    /*
     * critical section
     */
    use_resource(t_id);

    unlock(t_id);

    return NULL;
}

void use_resource(int t_id)
{
    counter++;
    usleep(rand() % 0xFFFF);
    counter++;
    usleep(rand() % 0xFFFF);
    counter--;
    usleep(rand() % 0xFFFF);
    counter--;
    usleep(rand() % 0xFFFF);
    counter++;
    usleep(rand() % 0xFFFF);

    printf("    %d\t     %d\t        %d\n", numbers[t_id], t_id, counter);

    fflush(stdout);
}

void lock(int t_id)
{
    choosing[t_id] = 1;
    int max = 0;

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        max = numbers[i] > max ? numbers[i] : max;
    }
    numbers[t_id] = max + 1;
    choosing[t_id] = 0;

    for (int j = 0; j < THREAD_COUNT; ++j)
    {
        while (choosing[j])
            ;

        while (numbers[j] != 0 && (numbers[j] < numbers[t_id] || (numbers[j] == numbers[t_id] && j < t_id)))
            ;
    }
}

void unlock(int t_id)
{
    numbers[t_id] = 0;
}