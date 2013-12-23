/*****
** http_client.c
** - implements the methods declared in http_client.h
** - 封装处理http协议
*/

#include "http_client.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 用get命令请求远程服务器的网页
char* http_do_get(int sockfd, char *path)
{
	char *tail_of_header =
		 "User-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.11 (KHTML, like Gecko) Ubuntu/11.10 Chromium/17.0.963.79 Chrome/17.0.963.79 Saf      ari/535.11\r\n"
		 "Content-Type: application/x-www-form-urlencoded\r\n"
		 "\r\n"; 
	char http_header[HEADER_SIZE];
	char recv_buf[BUF_SIZE];
	char *http_response = NULL;
	int offset;
	int tmp;

	//printf("GET %s HTTP/1.1\r\nHost: %s\r\n%s", path, HOST_NAME, tail_of_header);
	sprintf(http_header, "GET %s HTTP/1.1\r\nHost: %s\r\n%s", path, HOST_NAME, tail_of_header);
	send(sockfd, http_header, HEADER_SIZE, 0);
	http_response = (char *)malloc(sizeof (char) * RESPONSE_SIZE);//分配1500k内存用以接收一个网页
	if (http_response == NULL)
	{
		printf("================================fail to malloc 1500k to recv the page!\n");
		return NULL;
	}
	memset(http_response, 0, sizeof (char) * RESPONSE_SIZE);
	offset = 0;
	while (1)
	{
		if ((tmp = recv(sockfd, recv_buf, BUF_SIZE, 0)) == 0)
			break;
		else if (tmp == -1)
		{
			free(http_response);
			return NULL;
		}
		sprintf(http_response + offset, "%s", recv_buf);
		offset += tmp;
		if (offset > 1000000)
		{
			printf("large page!!!!!!!!!!!!!!!: %s\n", path);
			free(http_response);
			printf("larger than 1000k!\n");
			return NULL;
		}
	}

	http_response[offset] = '\0';//为这个字符串（里面存的是返回的http内容）添加‘\0’，这样用strstr函数的时候才能有尽头，字符串处理函数都要有结尾
	return http_response;
}

// 远程WEB服务器的http响应代码，如404
int http_response_status(char *http_response)
{
	int status;
	char buf[20];//get the string:"HTTP/1.1"

	sscanf(http_response, "%s%d", buf, &status);
	return status;
}
//http_response_body();//获取http响应的消息体字节流
