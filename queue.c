#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include"queue.h"

struct taskQueue *  queue_init()
{
    struct taskQueue *p=(struct taskQueue *)malloc(sizeof(struct taskQueue));

    p->front=NULL;
    p->rear=NULL;
    p->n=0;
    return p;
}


bool enqueue(struct taskQueue * head,void * (*data) (struct pathname*pa),struct pathname*arg)
{

    struct task* p = (struct task *)malloc(sizeof(struct task));
	if(p == NULL)
		return false;
	p->arg=arg;
	p->data=data;
    p->next=NULL;
	//尾插法
	if(head->n==0)
	{
		head->front = p;
		head->rear = p;
	}
	else
	{
		head->rear->next = p;
		head->rear = p;
	}
	
	head->n ++;
	
	return true;

}

struct task * dequeue(struct taskQueue * head)
{
    if(head->n==0)
    {
        return NULL;
    }
    struct task *p=head->front;
    head->front=head->front->next;
    head->n--;
    p->next=NULL;
    return p;
}

void queue_destroy(struct taskQueue * head)
{
    struct task *p=head->front;
    while(p)
    {
        head->front=head->front->next;
        free(p);
        head->n--;
        p=head->front;
    }

}