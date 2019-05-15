//localServer.c
#include "localServer.h"

#define BUFSIZE 1068
 
//初始化，负责初始化服务器。
void initServer()
{
    //将STDIN_FILENO修改为非阻塞
    int serFlags = fcntl(STDIN_FILENO, F_GETFL);
    serFlags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, serFlags);
    //以非阻塞只读的方式打开管道
    serFifo = open(SERVER_FIFO, O_RDONLY|O_NONBLOCK);
    if(serFifo < 0)
    {
        perror("SERVER OPEN:");
        exit(1);
    }
    printf("服务器已启动，正在监听...\n");
    startFlags = 1 - startFlags;
}
 
//负责接收客户端发送的包
void receiverPacket()
{
    char buf[BUFSIZE];
    MSP *msp;
    int len = read(serFifo, buf, sizeof(buf));

    if(len > 0)
    {
        msp = (MSP*)buf;
        parsingPacket(msp);
    }
}

//负责将客户端发送的包解析
void parsingPacket(MSP *msp)
{
    //根据相应的功能号，调用相应的函数。
    switch(msp->messageNo)
    {
        case 0:
            clientLogin(msp->senderName);
            break;
        case 1:
            messageSend(msp);
            break;
        case 2:
            clientQuit(msp->senderName);
            break;
    }
}

//负责客户端登陆，将客户端插入客户队列中,并创建私有管道
void clientLogin(char* loginName)
{
    //不能直接赋值，会造成浅拷贝
    strcpy(clientDueue[clientlen].clientName, loginName);
    char path[23] = "./";
    strcat(path, loginName);

	//确保创建的文件的权限为分配的权限
    umask(0);

    //创建管道
    mkfifo(path, 0777);

    //将管道的文件描述符存入数组中
    clientDueue[clientlen].fifoDes = open(path, O_WRONLY);
    char buf[]="您和服务器的连接已经成功建立,可以开始通讯了\n";
    write(clientDueue[clientlen].fifoDes, buf, sizeof(buf));

    //这里应该将管道创建为临时的，如果是使用数据库，可以创建为永久的
    unlink(path);

    //没有对cientlen进行限制
    ++clientlen;
}

//负责客户端的退出，将客户端从客户队列中删除,并删除创建的管道
void clientQuit(char* quitName)
{
    //最好是利用链表管理登录的客户机
    int i = 0;
    for(i = 0;i < clientlen; i++)
    {
        if(strcmp(quitName, clientDueue[i].clientName) == 0)
        {
       		//关闭对应的私有通道
            close(clientDueue[i].fifoDes);
            clientDueue[i].fifoDes = -1;
            clientDueue[i].clientName[0] = '\0';
            break;
        }
    }
    printf("%s已退出\n",quitName);
}

//负责将消息发送到对应的接受方
void messageSend(MSP *pMes)
{
    int i = 0;
    char* buf = (void*)pMes;
    if(strlen(pMes->receiverName) != 0)
    {
        //单发
        for(i = 0;i < clientlen; ++i)
        {
            if(strcmp(pMes->receiverName, clientDueue[i].clientName) == 0)
            {
                write(clientDueue[i].fifoDes, buf, BUFSIZE);
                break;
            }
        }
    }
    else
    {
        //群发
        for(i = 0;i < clientlen; ++i)
        {
            write(clientDueue[i].fifoDes, buf, BUFSIZE);
        }
    }
}
 
//负责处理输入的数据
void messageHanle(char* pMes)
{
    if(strcmp(pMes,"qquit")==0)
    {
        closeServer();
    }
    //可以继续增加一些命令（显示有几个客户端，客户端的管道描述符等）
}

//负责关闭服务器，关闭打开的管道和删除客户机列表
void closeServer()
{
    char buf[]="服务器维护中，请稍后登录。";
    int i = 0;
    for(i = 0;i < clientlen; ++i)
    {
        if(clientDueue[i].fifoDes != -1)
        {
            write(clientDueue[i].fifoDes, buf, strlen(buf));
            close(clientDueue[i].fifoDes);
        }
    }
    close(serFifo);
    startFlags = 1 - startFlags;
    printf("以关闭所有管道，服务器安全退出");
}


int main()
{
    initServer();
    char mes[1024];

    while(startFlags)
    {
        receiverPacket();
        if(scanf("%s", mes) != EOF)
        {
            messageHanle(mes);
        }
    }

    return 0;
}


