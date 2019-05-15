1_kill.c
    创建5个进程，并杀掉2号进程
        函数：kill abort raise

=====================================================================================

2_alarm.c
    测试1秒内计算机可以跑多少个数字
        一个进程有且只有一个alarm
	    alarm采用的是自然定时法，与进程状态无关。
	    计时结束后，进程收到SIGALRM信号后，默认动作终止
=====================================================================================
int setitimer(int which, 
			    const struct itimerval *new_value,
                struct itimerval *old_value);

new_value参数:
    用来对计时器进行设置，it_interval为计时间隔，it_value为延时时长
old_value参数:
	传出参数，通常用不上，设置为NULL，
	如果想知道定时器还有多久定到时间，再次调用setitimer函数，取出old_value可查看。
	If old_value is non-NULL, the buffer it points to is used to return the previous value of the timer 
	
settimer工作机制是:
    先对it_value倒计时，当it_value为零时触发信号，
    然后it_value重置为it_interval，继续对其倒计时，一直这样循环下去。

基于此机制，setitimer既可以用来延时执行，也可定时执行。

new_vlaue:
	假如it_value为0是不会触发信号的，所以要能触发信号，it_value得大于0；
	如果it_interval为零，只会延时，不会定时（也就是说只会触发一次信号)。


         
3_setitimer.c
    测试1秒内计算机可以跑多少个数字，使用setitimer

3_setitimer_signal.c
    使用signal函数捕捉信号，然后调用特定函数处理
    周期定时，每隔1s输出一个字符串
========================================    =============================================

4_sigpending.c
    函数：sigemptyset sigfillset sigaddset sigismemset信号集设定
          sigprocmask 映射到信号屏蔽字
          sigpending 获取未决信号集
    功能：修改屏蔽字，按键触发信号，获取并打印未决信号

=====================================================================================

5_sigaction1.c
    sigaction函数：信号捕捉函数
    1.捕捉SIGINT后(设定)，处理act.sa_handler期间，屏蔽act.sa_mask设置的屏蔽字SIGQUIT的干扰
    2.注册函数中sleep(7):连续发多个SIGINT只会记录一次 

        
=====================================================================================

6_mysleep.c
    函数：pause +　alarm
    功能:　实现系统睡眠函数sleep
    
    code：
        alarm(3);
        /////---pos1--///
        pause();
      
        SIGALRM信号不执行默认处理动作，而是调用捕捉函数func
        
    A:
        正常情况，alarm(5)后，后台数5s，然后调用pause进行阻塞，并等待信号唤醒。

        异常情况，alarm(5)后，紧接着失去了CPU，导致程序阻塞，而pause没有被调用。
            如果后台的5s数完后，程序回到刚刚失去CPU的地方继续向下执行(注意：这里pause没有被调用哦)，
            程序收到了alarm结束的SIGALRM信号，并调用信号注册函数。
            处理完后，程序继续向下执行，遇到pause又被挂起(阻塞)，但再也等不到信号
        
        解决办法是：
            alarm之前给内核区设置SIGALRM的信号屏蔽字，在sigsuspend调用时解除该信号的阻塞，并阻塞程序，
            保证阻塞的程序可以收到信号取消阻塞。

6_mysleep_question.c     
    极端情况的bug

    
6_mysleep_sigsuspend.c
    对代码进行强化
    
              
6_mypoweroff.c
    利用sigaction函数 + alarm函数 写关机脚本
    
                   
=====================================================================================   

7_sync_process.c
    父进程数1，传递信号SIGUSR1给子进程，子进程数2,传递信号SIGUSR2给父进程
    原程序(有bug)
        先发送SIGUSR1信号，再重置flag
        bug:刚发送了信号，还没来得及重置flag就又捕捉，导致卡死
    
    修复程序
        先重置flag，再发送信号
       
=======================================================================================

8_sigchild.c
    利用SIGCHLD信号的默认动作，掌握父进程使用捕捉函数回收子进程的方式。
    

=======================================================================================
9_sigqueue.c
    信号传参[很少用]
    
=======================================================================================
other

char *strtok(char s[], const char *delim);
    分解字符串为一组字符串。s为要分解的字符串，delim为分隔符字符串。



            
      
    
    
  
    
  
