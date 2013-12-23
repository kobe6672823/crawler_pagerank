#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#define HTTP_PORT 80
#define HOST_NAME "127.0.0.1"
#define HEADER_SIZE 1024
#define BUF_SIZE 1024
#define RESPONSE_SIZE 1024 * 1500//分配1500k的内存用以接收http返回内容（包括request头和html内容）

char* http_do_get(int sockfd, char *path);// 用get命令请求远程服务器的网页
int http_response_status(char *http_response);// 远程WEB服务器的http响应代码，如404
//http_response_body();//获取http响应的消息体字节流


#endif

