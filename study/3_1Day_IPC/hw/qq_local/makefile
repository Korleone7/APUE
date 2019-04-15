#取当前目录下所有的.c文件
src = $(wildcard *.c)

#把.c文件替换为.o文件
obj = $(patsubst %.c, %, $(src))

#自定义的一些变量
target=app

#makefile自己维护的一些变量
CC = gcc
CFLAGS = -Wall -g

#要生成的目标对象
$(target):$(obj)
# 如果加了下面这句话，代表要将$(obj)中的所有.o汇总成一个app
#	$(CC) $(obj) -o $(target)

# $< 依次取出依赖条件
# $@ 依次取出目标值
# $^ 规则中所有的依赖
%.o:%.c
	$(CC) $< -o $@ $(CFLAGS)

#清理
.PHONY:clean all
clean:
	-rm -rf $(obj) 

#测试
hello:
	echo "hello makefile"
