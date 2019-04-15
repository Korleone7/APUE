/******************************************************************************************************
 *													localClient.h
 *
 *******************************************************************************************************/

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define SERVER_FIFO "/home/dragon/Documents/study/3_1Day_IPC/hw/qq_local/ff1"
 
int linkFlags = 0;//连接标志
int serFifo;//公共管道文件描述符
int cliFifo;//客户端私有端道文件描述符
char clientName[20];//客户端名称
 
struct messagePacket
{
    int messageNo;//消息编号
    char senderName[20];//消息发送方
    char receiverName[20];//消息接收方
    char data[1024];//数据采用定长消息
};
 
typedef struct messagePacket MSP;
 
//初始化客户端
void initClient();
//登陆服务器
void loginServer();
//向服务器发送消息指令
void sendSerMes(int mesNO);
//处理用户输入的数据
void messageHanle(char* pMes);
//向其他用户发送消息
void sendMessage(char* receiverName,char* data);
//接收消息
void receiverMes();
//关闭客户端
void closeClient();


