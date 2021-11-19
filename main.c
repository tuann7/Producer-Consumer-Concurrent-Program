#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 8 

int buffer[BUFFER_SIZE];
int add = 0;
int pop = 0;
int bufferStatus = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;

typedef struct Thread
{
    int n;
    int i;
    int d;
}Thread;

int bufferIsFull();
int bufferIsEmpty();
void* producer(void* param);
void* consumer(void* param);

int main(int argc, char* argv[]) 
{
    if (argc != 5)
    {
        printf("4 arguments expected\n");
    }
    else
    {
        int p = atoi(argv[1]); // number of producers
        int c = atoi(argv[2]); // number of consumers
        int i = atoi(argv[3]); // the number of items produced by each producer
        int d = atoi(argv[4]); // Delay option

        pthread_t producer_thread[p];
        pthread_t consumer_thread[c];

        int counter;
        for (counter = 0; counter < p; counter++)
        {
            struct Thread* thread_prop = (struct Thread*)malloc(sizeof(struct Thread));
            thread_prop->n = counter;
            thread_prop->i = i;
            thread_prop->d = d;
            pthread_create(&producer_thread[counter], NULL, producer, thread_prop);
        }

        for (counter = 0; counter < c; counter++)
        {
            struct Thread* thread_prop = (struct Thread*)malloc(sizeof(struct Thread));
            thread_prop->n = counter;
            thread_prop->i = p * i / c;
            thread_prop->d = d;
            pthread_create(&consumer_thread[counter], NULL, consumer, thread_prop);
        }

        for (counter = 0; counter < p; counter++)
        {
            pthread_join(producer_thread[counter], NULL);
        }

        for (counter = 0; counter < c; counter++)
        {
            pthread_join(consumer_thread[counter], NULL);
        }
    }
    return(0);
}

int bufferIsFull()
{
    if (bufferStatus >= BUFFER_SIZE)
    {
        return 1;
    }
    return 0;
}

int bufferIsEmpty()
{
    if (bufferStatus == 0)
    {
        return 1;
    }
    return 0;
}

void* producer(void* param)
{
    struct Thread *thread_prop = (struct Thread *)param;

    int counter;
    for (counter = 0; counter < thread_prop->i; counter++)
    {
        pthread_mutex_lock(&mutex);

        while (bufferIsFull())
        {
            pthread_cond_wait(&notfull, &mutex);
        }

        /* Produces an item */
        int item = thread_prop->n * thread_prop->i + counter;
        add = (add + 1) % BUFFER_SIZE;
        buffer[add] = item;
        bufferStatus += 1;

        printf("producer_%d produced item %d\n", thread_prop->n, item);

        pthread_cond_signal(&notempty);
        pthread_mutex_unlock(&mutex);

        // Delay option d = 0 delay added to producer
        if (thread_prop->d == 0)
        {
            usleep(500000);
        }
    }
    pthread_exit(0);
}

void* consumer(void* param)
{
    struct Thread *thread_prop = (struct Thread *)param;
    int counter;
    for (counter = 0; counter < thread_prop->i; counter++)
    {
        pthread_mutex_lock(&mutex);

        while (bufferIsEmpty())
        {
            pthread_cond_wait(&notempty, &mutex);
        }

        /* consumes an item */
        pop = (pop + 1) % BUFFER_SIZE;
        int item = buffer[pop];
        bufferStatus -= 1;

        printf("consumer_%d consumed item %d\n", thread_prop->n, item);

        // Delay option d = 1 delay added to consumer
        if (thread_prop->d == 1)
        {
            usleep(500000);
        }
        pthread_cond_signal(&notfull);
        pthread_mutex_unlock(&mutex);       
    }

    pthread_exit(0);
}
