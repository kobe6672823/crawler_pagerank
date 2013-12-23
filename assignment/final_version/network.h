#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>

#define TIMEOUT 10//设置接收超时时间，10s 

int open_tcp(char *host_name, int port);
/*
nsend();//发送字节到网络
nrecv();//从网络接收字节
recv_line();//从socket接收一行，以换行结束
close();//socket中已经有这个函数了，没有必要再写一个
*/
#endif
