/********************************************************************************************************************
                                                       阶段综合练习二
                                                    实现简单的交互式shell。
使用已学习的各种C函数实现一个简单的交互式Shell，要求：
    1、给出提示符，让用户输入一行命令，识别程序名和参数并调用适当的exec函数执行程序，待执行完成后再次给出提示符。
	
    2、该程序可识别和处理以下符号：
        1) 简单的标准输入输出重定向：< or >

		2) 管道(|): Shell进程先调用pipe创建管道，然后fork出两个子进程。
           一个子进程关闭读端，调用dup2将写端赋给标准输出，另一个子进程关闭写端，调用dup2把读端赋给标准输入，
           两个子进程分别调用exec执行程序，而Shell进程把管道的两端都关闭，调用wait等待两个子进程终止。
           类似于“兄弟进程间ls | wc –l”练习的实现。
    

	3、你的程序应该可以处理以下命令：
        ○ ls △ -l △ -R ○  > ○ file1 ○
        ○ cat ○ < ○ file1 ○ | ○ wc △ -c ○ > ○ file1 ○
      注：○ 表示零个或多个空格，△ 表示一个或多个空格
	
	  
	4.个人bug：参数和命令之间至少保持一个空格，已知思路但懒得修复


实现步骤： 
    1. 接收用户输入命令字符串，拆分命令及参数存储。（自行设计数据存储结构）
    2. 实现普通命令加载功能 
    3. 实现输入、输出重定向的功能 
    4. 实现单重管道 
    5. 支持多重管道

*********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>


typedef struct cmd_t{
	char cmd[1000];
	char arg[10][10];
	char *pp_cmd[10];
	char *pp_arg[10];
	char *pl_cmd[10];
	char *pl_arg[10];
}cmd_t;



int main(int argc, char *argv[]) {
	char str[1000];
	int i = 0;//遍历全部字符串
	int j = 0, k = 0, l = 0, m = 0, pn = 0;//参数
	int fflag = 1;//0:归零操作 1:采集程序 2:采集参数
	int fd, fd2, ofd, ifd;
	int reout = 0, rein = 0;//输出/输入重定向：默认0,
	int pfd[2];
	cmd_t ct;
	pid_t p;

	ofd = dup(STDOUT_FILENO);
	ifd = dup(STDIN_FILENO);

	while(1) {
		i = 0; j = 0; k = 0; m = 0; fflag = 1; pn = 0;
		memset(str, 0, 1000);	
		memset(&ct, 0, sizeof(ct));

		printf("Input ur Order:");
		fgets(str, 1000, stdin);//接收用户输入

		//识别程序名和参数		 
		//	1.通过空格区分程序和参数
		for(i = 0; i < strlen(str) - 1; ++i) {
			if(str[i] != ' ' && fflag == 1) {//程序
				ct.cmd[j++] = str[i]; 
				if(str[i+1] == ' ') {
					fflag = 0;
				}
				continue;
			}	
					
			if(str[i] != ' ') {
				if(fflag == 0) {//第一次
					m++;
				}
				fflag = 2;
				ct.arg[m][k++] = str[i];
				//printf("m% d __%c__\n", m, ct.arg[m][k-1]);
				if(str[i+1] != ' ') {
					continue;
				}
				else {
					l = i;
					//printf("l %d len %d\n", l, (int)strlen(str)-1);
					while(l++ < strlen(str) - 1) {
						if(str[l] != ' ' && str[l] != '\n') {
							++m;
							k = 0;
							break;
						}	
					}
				}
			}
		}
		
		//printf("m = %d \n", m);

		//参数整合
		char *arg_v[100];
		//memset(*arg_v, 0, 100);
		arg_v[0] = ct.cmd;
		for(k = 1; k <= m; ++k) {
			if(m == 0) {//说明没有参数
				k = 2;
				break;
			}
			arg_v[k] = ct.arg[k];
			printf("ct.cmd %s arg_v[%d] %s\n", ct.cmd, k, arg_v[k]);

			if(*arg_v[k] == '>') {//输出重定向
				reout = 1;
				arg_v[k] = NULL;
			}
			//这里必须用else，如果两个if的话，下面的if,就会发生取NULL的地址报段错误
			else if(*arg_v[k] == '<') {//输入重定向
				rein = 1;
				arg_v[k] = NULL;
			}
			//统计管道数 ls | wc -l
			else if(*arg_v[k] == '|') {
				pn++;
			}
		}
		
		//printf("k %d pn %d\n", k, pn);
		//不含重定向，尾部+NULL
		if(reout != 1 && rein != 1) {
			arg_v[k] = NULL;
		}
		if(pn == 0 || pn != 0){//无论是否包含管道 
			arg_v[k] = NULL;
		} 

		if(reout == 1) {//输出重定向	
			fd = open(arg_v[k-1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
			if(fd < 0){
				perror("open >  error");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
		}

		if(rein == 1) {//输入重定向	
			fd2 = open(arg_v[k-1], O_RDWR);
			if(fd < 0){
				perror("open <  error");
				exit(1);
			}
			dup2(fd2, STDIN_FILENO);
		}

		//单重管道 ls | wc -l
		int pre = 1, j = 0;
		if(pn > 0) {
			for(i = 0; i <= m+1; ++i) {
				//printf("pipe arg_v[%d] %s \n",i, arg_v[i]);
				if(pre == 1 && *arg_v[i] != '|') {
					if(i == 0) {
						ct.pp_cmd[i] = arg_v[i];
						ct.pp_arg[i] = arg_v[i];
					}
					else {
						ct.pp_arg[i] = arg_v[i];
					}
				}
				if(pre == 1 && *arg_v[i] == '|') {
					ct.pp_cmd[i] = NULL;
					pre = 0;
					continue;
				}
				if(pre == 0) {
					if(j == 0) {
						ct.pl_cmd[j] = arg_v[i];
						ct.pl_arg[j++] = arg_v[i];
					}
					else {
						ct.pl_arg[j++] = arg_v[i];
					}
					//printf("i %d j %d ==== %s \n",i, j-1, arg_v[i]);
				}
			}

			if(pipe(pfd) == -1) {
				perror("pipe err ");
				exit(-1);
			}
		}

		//因为exec族执行成功后，不会返回主程序，
		//因此需要fork出一个子进程让他执行命令，父进程回收
		//这样就可以实现循环输入命令
		for(i = 0; i < pn+1; ++i) {
			p = fork();
			if(p == -1) {
				perror("fork err");
				exit(-1);
			}
			if(p == 0) {
				break;
			}
		}
		
		if(i == pn+1) {//父进程
			if(reout == 1) {//输出重定向结束，恢复输出到屏幕
				reout = 0;
				dup2(ofd, STDOUT_FILENO);
				close(fd);
			}
					
			if(rein == 1) {//输入重定向结束，恢复从键盘输入
				rein = 0;
				dup2(ifd, STDIN_FILENO);
				close(fd2);
			}
			
			if(pn != 0) {//管道用完关闭
				close(pfd[0]);//读端不关闭，满则write阻塞
				close(pfd[1]);//写端不关闭，read会等待阻塞
			}

			for(j = 0; j < pn+1; j++)	
				wait(NULL);
		} 
		else if(i == 0 && pn == 0) {//子进程且没有管道
			//并调用适当的exec函数执行程序
			execvp(ct.cmd, arg_v);	
			perror("execlp err \n");
		}
		else if(i == 0 && pn == 1) {
			//带管道的子进程1
			//printf("1st child\n");
			//printf("%s %s\n", *ct.pp_cmd, ct.pp_arg[0]);
			
			//close read 0
			close(pfd[0]);
			dup2(pfd[1], STDOUT_FILENO);
			execvp(*ct.pp_cmd, ct.pp_arg);
			perror("execvp err 1st ");
		}
		else if(i == 1 && pn == 1) {
			//带管道的子进程2
			//printf("2nd child\n");
			//printf("%s %s\n", *ct.pl_cmd, ct.pl_arg[0]);
			
			//close wirte 1
			close(pfd[1]);
			dup2(pfd[0], STDIN_FILENO);
			execvp(*ct.pl_cmd, ct.pl_arg);
			perror("execvp err 2nd ");
		}
	}

	return 0;
}


