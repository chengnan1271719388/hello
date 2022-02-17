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

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("输入有误\n");
		return 0;
	}

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("socket error");
		return 0;
	}

    struct sockaddr_in addr;/ /服务端的网络地址		
	//void *memset(void *s, int c, size_t n); //用于设置一块内存为指定值c
	memset(&addr,0,sizeof(addr));//把addr变量内存所有字节设置为 0
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));//指定端口号，并进行字节序的转换
	inet_ato n(argv[1],&addr.sin_addr);//指定IP地址

	int r = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	if(r == -1)
	{
		perror("connect error");
		close(sockfd);
		return 0;
	}
    while (1)
    {
        //查看文件目录

        //下载文件

        //上传文件

        //退出客户端


    }
    close(fd);
	close(sockfd);
}