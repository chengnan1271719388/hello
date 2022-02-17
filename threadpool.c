#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include"threadpool.h"

struct threadPool* threadPool_init(int nums)
{
    struct threadPool * p=(struct threadPool *)malloc(sizeof(struct threadPool));
    p->threads=nums;
    p->taskHead=queue_init();
    p->tids=(pthread_t *)malloc(sizeof(pthread_t)*nums);
    pthread_mutex_init(&(p->mutex),NULL);
    pthread_cond_init(&(p->cond),NULL);
    return p;
}


bool destroy_threadPool(struct threadPool * p)
{
    if(p==NULL)
    {
        return false;
    }
    queue_destroy(p->taskHead);

    free(p->tids);

    pthread_mutex_destroy(&(p->mutex));

    free(p);

    return true;
}