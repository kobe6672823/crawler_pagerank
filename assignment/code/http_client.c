/*****
** http_client.c
** - implements the methods declared in http_client.h
** - 封装处理http协议
*/

#include "http_client.h"

http_do_get();// 用get命令请求远程服务器的网页
http_response_status();// 远程WEB服务器的http响应代码，如404
http_response_body();//获取http响应的消息体字节流