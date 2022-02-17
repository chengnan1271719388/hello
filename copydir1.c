#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include "queue.h"
#include "threadpool.h"

struct threadPool*Pool;
int number=0;

void* copy_file(struct pathname* p)
{
	
		int fd1 = open(p->srcpathname,O_RDONLY);
		if(fd1 == -1)
		{
			//源文件打开失败，就结束操作
			perror("open error");
			return NULL;
		}

		int fd2 = open(p->destpathname,O_WRONLY | O_CREAT | O_TRUNC,0664);
		if(fd2 == -1)
		{
			//目的文件打开失败，就结束操作
			perror("open error");
			close(fd1);
			return NULL;
		}

		#if 0
		int size = lseek(fd1,0,SEEK_END);//求源文件的大小，目的是为了一次把文件内容读取出来
		char buf[size];
		lseek(fd1,0,SEEK_SET);//再把光标移到开头位置
		read(fd1,buf,size);//一次把文件内容全部读取出来
		write(fd2,buf,size);

		//如果是一个很大很大的文件，那么可能会造成内存不足
		#else
		char buf[1024];
		int r;
		while(1)
		{
			//什么时候退出？读取完毕就退出
			//怎么判断读取完毕
			r = read(fd1,buf,1024);
			if(r == 0)
			{
				break;
			}
			write(fd2,buf,r);
		}

		#endif

		close(fd1);
		close(fd2);
}

bool copy_dir(const char * src,const char * dest)
{
	int r = mkdir(dest,0764);
	if(r == -1 && errno != EEXIST)//创建失败并且错误原因不是文件已存在
	{
		perror("mkdir error");
		return false;
	}

	//读取源目录

	//1,打开目录
	DIR * pdir = opendir(src);
	if(pdir == NULL)
	{
		perror("opendir error");
		return false;
	}

	//读取目录项
	struct dirent * p ;
	while(1)
	{
		p = readdir(pdir);
		if(p==NULL)
			break;

		//跳过当前目录 . 及上一层目录 ..
		//if(p->d_name == "." || p->d_name == "..")
		if(strcmp(p->d_name,".")==0 || strcmp(p->d_name,"..")==0)
		{
			continue;
		}

		char srcbuf[200] = "";//保存元文件路径名
		strcpy(srcbuf,src);
		strcat(srcbuf,"/");
		strcat(srcbuf,p->d_name);
		//printf("debug:%s\n",srcbuf);
		char destbuf[200] = "";
		strcpy(destbuf,dest);
		strcat(destbuf,"/");
		strcat(destbuf,p->d_name);
		//printf("debug:%s\n",destbuf);
		
		struct stat s;
		stat(srcbuf,&s);//获取源文件的属性
		if((s.st_mode & S_IFMT) == S_IFDIR)//目录文件，递归调用
		{
			copy_dir(srcbuf,destbuf);
		}
		else//其他文件，加入到任务队列
		{
			
            //copy_file(srcbuf,destbuf);
			struct pathname *pa = (struct pathname *)malloc(sizeof(struct pathname));
			strcpy(pa->srcpathname,srcbuf);//1.txt  2.txt
			strcpy(pa->destpathname,destbuf);//1.txt 2.txt
			
			//pthread_create(&tid[threads],NULL,copy_file,(void *)pa);
			//threads++;
            pthread_mutex_lock(&(Pool->mutex));
            enqueue(Pool->taskHead,copy_file, pa);
            pthread_mutex_unlock(&(Pool->mutex));

            if(number<5&&number<Pool->taskHead->n)
            {
                pthread_cond_signal(&(Pool->cond));
            }
		}
	}
	
}

void*func(void*arg)
{
	 while(1)
	{
		pthread_mutex_lock(&(Pool->mutex));
		while(Pool->taskHead->n==0)
		{
			pthread_cond_wait(&(Pool->cond),&(Pool->mutex));
		}
		struct task*p=dequeue(Pool->taskHead);//出队
		number++;
		pthread_mutex_unlock(&(Pool->mutex));
		(p->data)(p->arg);
		number--;
		free(p);
	}

}

int main(int argc,char * argv[])
{
    Pool=threadPool_init(5);
	int i;
    for(i=0;i<5;i++)
    {
        pthread_create(Pool->tids+i,NULL,func,NULL);
    }
    copy_dir(argv[1],argv[2]);
	while(1)
	{
		if(number==0&&Pool->taskHead->n==0)//没有线程在运行且队列中无任务
		{
			for(i=0;i<5;i++)
			{
				pthread_cancel(Pool->tids[i]);//关闭线程
			}
			destroy_threadPool(Pool);//销毁线程池
			break;
		}
	}

	return 0;
}
