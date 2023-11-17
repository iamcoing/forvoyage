#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "crc.h"

#define WAITNUM 5
#define ERR_MSG(msg) do{\
	printf("%s:%s:%d", __FILE__, __func__, __LINE__);\
	perror(msg);\
}while(0)

typedef struct{
	char buf[64];
	unsigned short crc;
}MSG;

int main(int argc, const char *argv[])
{
	MSG msg;
	unsigned short crc = 0;
	int sfd, efd, newfd;
	ssize_t ret = 1;
	//搭建UDP模型
	//创建报式套接字
	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(EOF == sfd){
		ERR_MSG("socket");
		return sfd;
	}

	//允许端口快速重用
	int opt = SO_REUSEADDR;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//填充地址信息结构体
	struct sockaddr_in sin, cin;
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	sin.sin_port		= htons(atoi(argv[2]));
	sin.sin_family 		= AF_INET;
	socklen_t addrlen	= sizeof(cin);
	//绑定信息结构体
	if(bind(sfd, (struct sockaddr *)&sin, addrlen) < 0){
		ERR_MSG("bind");
		return -1;
	}

 	//将sfd设置为非阻塞IO
	fcntl(sfd, F_SETFD, fcntl(sfd, F_GETFD) | O_NONBLOCK);
	//创建epoll模型
	efd = epoll_create(6);
	if(EOF == efd){
		ERR_MSG("epoll create");
		return efd;
	}

	//添加epoll的读写事件
	//创建一个epoll事件的变量用来注册事件
	struct epoll_event event;
	//创建一个epoll事件的数组，用来保存触发的事件
	struct epoll_event revents[WAITNUM];
	event.events  = EPOLLET | EPOLLIN;
	event.data.fd = sfd; 
	if(epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event) < 0){
		ERR_MSG("epoll contrl");
		return -1;
	}
	int maxevents = WAITNUM;
	memset(msg.buf, 0, sizeof(msg.buf));

	//执行循环等待客户端收发数据
	while(1){
		newfd = epoll_wait(efd, revents, maxevents, -1);
	//判断读写事件的产生
		for(int i = 0; i <= newfd; i++){
			if( revents[i].events & EPOLLIN){
				if( (sfd = revents[i].data.fd) < 0)
					continue;

				ret = recvfrom(sfd, &msg, sizeof(msg), 0, (struct sockaddr*)&cin, &addrlen);
				if( ret < 0 ){
						printf("recvfrom error\n");
						continue;		
				}else if(ret == 0){
					ERR_MSG("connfd");
					close(sfd);
					revents[i].data.fd = -1;
				}else{

					crc = CheckCRC16(msg.buf, strlen(msg.buf), CRC16MAXIM);

					if(crc == msg.crc)
						printf("[%s:%d] recv: %s, crc: %#x\n", inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), msg.buf, msg.crc);
				}
				event.data.fd = sfd;
				event.events  = EPOLLOUT | EPOLLET;
				epoll_ctl(efd, EPOLL_CTL_MOD, sfd, &event);
			}else if(revents[i].events & EPOLLOUT){
				if(revents[i].data.fd == -1){
					continue;
				}else{
					sfd = revents[i].data.fd;
					event.data.fd = sfd;
					event.events  = EPOLLIN | EPOLLET;
					bzero(msg.buf, sizeof(msg.buf));
					strcpy(msg.buf, "hello world");
					
					msg.crc = CheckCRC16(msg.buf, strlen(msg.buf), CRC16MAXIM);

					ret = sendto(sfd, &msg, sizeof(msg), 0, (struct sockaddr *)&cin, addrlen);
					if(ret < 0){
							perror("sendto");
							continue;
					}
					epoll_ctl(efd, EPOLL_CTL_MOD, sfd, &event);
				}
			}
		}
	}

	return 0;
}

