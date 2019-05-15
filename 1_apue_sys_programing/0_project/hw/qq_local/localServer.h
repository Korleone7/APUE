//localServer.h
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
 
#define SERVER_FIFO "/home/dragon/Documents/study/3_1Day_IPC/hw/qq_local/ff1"
 
struct client
{
    char clientName[20];//客户端名字
    int fifoDes;//私有管道的描述符
};
 
typedef struct client CL;

//用来记录客户机的数量
int clientlen = 0;

//利用数组将存储客户队列（不方便，而且会浪费），可以改造为链表（最好）。
CL clientDueue[100];
 
struct messagePacket
{
    int messageNo;//消息编号
    char senderName[20];//消息发送方
    char receiverName[20];//消息接收方
    char data[1024];//数据采用不定长消息
};
 
typedef struct messagePacket MSP;
 
//公共管道
int serFifo;
//服务器启动标志
int startFlags=0;
//初始化，负责初始化服务器。
void initServer();
//负责接收客户端发送的包
void receiverPacket();
//负责将客户端发送的包解析
void parsingPacket(MSP *msp);
//负责客户端登陆，将客户端插入客户队列中,并创建私有管道
void clientLogin(char* loginName);
//负责将消息发送到对应的接受方
void messageSend(MSP *pMsp);
//负责客户端的退出，将客户端从客户队列中删除,并删除创建的管道
void clientQuit(char* quitName);
//负责关闭服务器，关闭打开的管道和删除客户机列表
void closeServer();
//负责处理输入的数据
void messageHanle(char* pMes);

