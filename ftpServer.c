#include<stdio.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ROOTPATH "/mnt/hgfs/CS21241"

int main(int argc ,char *argv[])
{
	if(argc != 3)
	{
		printf("输入有误\n");
		return 0;
	}

	//1,创建套接字
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("socket error");
		return 0;
	}
     //2,绑定服务端自己的网络地址
	//定义并设置服务端自己的网络地址
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));//把addr变量内存所有字节设置为 0
	
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[2]));//指定端口号，并进行字节序的转换
	inet_aton(argv[1],&saddr.sin_addr);//指定IP地址

	int r = bind(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
	if(r==-1)
	{
		perror("bind error");
		close(sockfd);
		return 0;
	}

	//3,监听
	r = listen(sockfd,3);
	if(r==-1)
	{
		perror("listen error");
		close(sockfd);
		return 0;
	}
	int connfd;
	struct sockaddr_in caddr;//用来保存向我（服务端）发送连接请求的客户端的网络地址
	int len = sizeof(caddr);
	while(1)
	{
		connfd = accept(sockfd,(struct sockaddr*)&caddr,&len);
		if(connfd == -1)
		{
			perror("accept error");
			close(sockfd);
			return 0;
		}

		printf("ip地址为%s,端口号为%hu的客户端与我建立了连接\n",inet_ntoa(caddr.sin_addr),
				caddr.sin_port);

		pid_t pid = fork();
		if(pid == 0)//子进程用来与客户端传输文件
		{
			close(sockfd);
			                                                                         
            close(connfd); 
        }
        else if(pid > 0)//父进程继续等待连接别的客户端
		{
			close(connfd);
		}
		else
		{
			perror("fork error");
			return 0;
		}
    }
    close(sockfd);
    return 0;
}