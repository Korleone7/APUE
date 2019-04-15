 /study/3_1Day_IPC/mmap
 
 fork_share_fd.c
    父子进程共享打开的文件描述符------使用文件完成进程间通信
 
 mmap.c
    mmap测试案例01
    在进程内创建一个test.txt的mmap共享存储映射区，
    并通过在映射区内写入数据后，同步到磁盘中
    7个思考题

fork_mmap.c
    创建一个临时文件(unlink close)用于创建mmap共享存储映射区
    并用于父子间进程通信

fork_map_anon_linux.c
    linux下，不用临时文件来创建mmap共享存储映射区
    MAP_SHARED | MAP_ANONYMOUS
    
fork_map_anon.c
    类Unix操作系统下，不用临时文件来创建mmap共享存储映射区
    fd = open("/dev/zero", O_RDWR);
    p = mmap(NULL, 400, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
   
./mmap_shared
	非血缘进程之间通过mmap进行数据交换

./file_IPC
	对比fork_share_fd.c，深刻理解“打开文件也是相当于创建了一个mmap映射区”
	通过strace ./test1可以看到test1执行过程中调用了哪些系统调用
	

smalloc.c
    参考malloc和free函数，自定义函数smalloc和sfree来完成映射区的建立和释放 

dict.txt
    一个较大的文件用于测试mmap函数的offset
   
 

