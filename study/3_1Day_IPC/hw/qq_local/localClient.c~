/******************************************************************************************************
 *File: localClient.c
 *
 *******************************************************************************************************/

#include "localClient.h"

#define BUFSIZE 1068

//初始化客户端
void initClient()
{
    loginServer();
    //将连接标志置为1.
    linkFlags = 1 - linkFlags;
    //将STDIN文件属性修改为非阻塞
	//	1.获取file flag
	//	2.修改flag为非阻塞
	//	3.设置flag
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);
}
 
//登陆服务器
//	1.打开与服务器之间的公有管道，并写数据
//	2.发送0号指令，告诉服务器已经登录
//	3.创建私有pipe
void loginServer()
{
    memset(clientName, 0, 20);
    //write(STDIN_FILENO,clientName,20);
    printf("请输入客户端名称(不超过20个字符):\n");
    scanf("%s", clientName);    

    
    //fgets(clientName,20,stdin);
	//printf("%s\n", clientName);
    serFifo = open(SERVER_FIFO, O_WRONLY|O_NONBLOCK);
    if(serFifo < 0) {
        perror("open server fifo");
        exit(1);
    }
	
	//发送0号指令:新用户登录
	sendSerMes(0);
	//字符串拼接用于打开私有fifo pipe
    char path[23]="./";
    strcat(path, clientName);
    //测试管道文件是否存在 [在服务器端创建私有管道]
    while(access(path, F_OK) != 0);
	//打开私有fifo pipe
    cliFifo = open(path, O_RDONLY|O_NONBLOCK);
    if(cliFifo < 0) {
        perror("open client fifo");
    }
    printf("私有管道创建成功\n");
}

//向服务器发送消息指令
//	0.登录
//	1.发送消息
//	2.客户端退出
void sendSerMes(int mesNO)
{
    MSP msp;
    char *buf;

    msp.messageNo = mesNO;
    strcpy(msp.senderName, clientName);
    buf = (void*)&msp;
    write(serFifo, buf, sizeof(msp));
}
 
//处理用户输入的数据
void messageHanle(char* pMes)
{
    //将“qquit”设置为退出消息
    if(strcmp(pMes, "qquit") == 0)
    {
        sendSerMes(2);
        closeClient();
        return;
    }

    //发送数据格式为：接受者姓名:消息内容
    //如果数据不符合规范，则将消息转为群发。
    int i=0;
    int j=0;
    char receiverName[20];
    char data[1024];
    
	//分离出接收方的字符串
	while(pMes[i] != '\0' && pMes[i] != ':')
    {
        receiverName[i] = pMes[i];
        ++i;
    }
    receiverName[i] = '\0';
    if(pMes[i] == ':')
    {
        //将：跳过
        ++i;
    }
    else
    {
        i=0;
        receiverName[0] = '\0';
    }
	//分离出发送的数据段
    while(pMes[i] != '\0')
    {
       data[j++] = pMes[i++];
    }
    data[j] = '\0';
    sendMessage(receiverName, data);
}
 
//向其他用户发送消息
void sendMessage(char* receiverName,char* data)
{
    MSP msp;
    char *buf;
    msp.messageNo = 1;

    strcpy(msp.senderName, clientName);
    strcpy(msp.receiverName, receiverName);
    strcpy(msp.data, data);
    buf = (void*)&msp;
    write(serFifo, buf, sizeof(msp));
}
 
//客户端接收来自服务器的消息
void receiverMes()
{
    char buf[BUFSIZE];
    int len = read(cliFifo, buf, sizeof(MSP));
    MSP *pMes = NULL;
    pMes = (void*)buf; 

    if(len > 0 && pMes->messageNo == 1)//客户端消息
    {
        printf("%s:%s\n",pMes->senderName,pMes->data);
    }
    else if(len > 0)//服务器的提示
    {
        printf("系统提示:%s",buf);
    }
}
 
//关闭客户端
void closeClient()
{
    //将连接标志置为0
    linkFlags = 1 - linkFlags;
    //关闭私有管道
    close(cliFifo);
    //关闭公共管道
    close(serFifo);
    printf("已关闭所有管道，客户端已安全退出\n");
}
 
int main() {
    initClient();

    char mesBuf[1024];

    while(linkFlags)
    {
        //scanf()默认遇空格终止scanf("%49[^\n]",mesBuf)!=EOF
        //int len=write(STDIN_FILENO,mesBuf,BUFSIZE);
        if(scanf("%s", mesBuf) != EOF)
        {
            messageHanle(mesBuf);
        } 
        receiverMes();
    }
	
    return 0;
}

/*===================		EOF		=========================*/
