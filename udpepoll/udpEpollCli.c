#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include "crc.h"

#define ERR_MSG(msg){\
	printf("__%d__\n", __LINE__);\
	perror(msg);\
}

typedef struct{
	char buf[64];
	unsigned short crc;
}MSG;

int main(int argc, const char *argv[])
{
	MSG msg;
	unsigned short crc = 0;
	//创建报式套接字
	int sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0)
	{
		ERR_MSG("socket");
		return -1;
	}
	
	//允许端口快速重用
	int opt = SO_REUSEADDR;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//填充结构体
	struct sockaddr_in sin;
	socklen_t addrlen = sizeof(sin);
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	sin.sin_port		= htons(atoi(argv[2]));
	sin.sin_family 		= AF_INET;

	ssize_t res = 0;

	while(1)
	{
		static int count;
		bzero(msg.buf, sizeof(msg.buf));
		printf("请输入>>\n");
		fgets(msg.buf, sizeof(msg.buf), stdin);
		msg.buf[strlen(msg.buf) - 1] = '\0';

		msg.crc = CheckCRC16(msg.buf, strlen(msg.buf), CRC16MAXIM);

		if(sendto(sfd, &msg, sizeof(msg), 0, (struct sockaddr *)&sin, addrlen) < 0)
		{
			ERR_MSG("sendto");
			return -1;
		}

		res = recvfrom(sfd, &msg, sizeof(msg), 0, (struct sockaddr *)&sin, &addrlen);
		if(0 == res)
		{
			printf("link break\n");
			break;
		}

		crc = CheckCRC16(msg.buf, strlen(msg.buf), CRC16MAXIM);
		
		if(crc == msg.crc)
			printf("recv: %s, crc: %#x\n", msg.buf, msg.crc);
	}

	close(sfd);

	return 0;
}
