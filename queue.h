#ifndef __QUEUE_H__
#define __QUEUE_H__

struct pathname
{
	char srcpathname[300];
	char destpathname[300];
};
struct task//任务节点类型
{
	void * (*data) (struct pathname*pa);
	//函数指针，指向一个函数，该函数就是用来执行任务的
	//至于该函数需要什么参数及有什么返回值，根据你的实际情况决定
	//为了通用，-》 void *的参数，有一个void*的返回值
	struct pathname* arg;//data指向的函数的参数
	
	struct task * next;//指针域，保存下一个队列节点的地址
};
		
		
//任务队列头结点类型
struct taskQueue
{
	struct task * front ;//保存队头节点地址
	struct task * rear ;//保存队尾节点地址
	int n;//保存该任务队列中 任务节点的个数
};


//初始化一个队列
struct taskQueue *  queue_init();


//入队
bool enqueue(struct taskQueue * head,void * (*data) (struct pathname*pa),struct pathname*arg);



//出队
//返回值：成功返回队头节点地址，失败返回NULL
struct task * dequeue(struct taskQueue * head);


void queue_destroy(struct taskQueue * head);


#endif
