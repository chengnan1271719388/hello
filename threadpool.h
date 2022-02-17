#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include<pthread.h>
#include"queue.h"
		

//线程池类型
struct threadPool
{
	int threads;//保存线程池中线程的个数
	//pthread_t tid[N];//线程id，用数组表示的话，不好确定线程的个数
	pthread_t *tids;//tids是指针，到时为其动态分配一定字节的空间，用来保存线程池中各个线程的id
	
	struct taskQueue * taskHead;
	
	//线程池是一个典型的消费者生产者模型
	//线程池中的线程从任务队列中 取下任务（出队）时会产生竞争
	//所以需要线程互斥锁
	pthread_mutex_t mutex;
	
	//线程池中的线程怎么知道有没有任务呢？
	//1，轮询		2，条件变量 <-
	pthread_cond_t cond;
};

struct threadPool* threadPool_init(int nums);

bool destroy_threadPool(struct threadPool * p);

#endif
		

