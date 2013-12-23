#ifndef _NETWORK_H_
#define _NETWORK_H_



open_tcp(); //建立一个到远程服务器的socket连接
nsend(); 发送字节到网络
nrecv();//发送网络字节流
recv_line();//从socket接收一行以回车键结束
close();//关闭socket

#endif
