#obj=main.o add.o sub.o mul.o

target=app
#makefile自己维护的一些变量
CC=gcc
CPPLFAGS = -I
CFLAGS = -Wall -g -c
LDFLAGS = -L -l

#makefile中函数的使用
src = $(wildcard ./*.c)
obj = $(patsubst ./%.c, ./%.o, $(src))


app:$(obj)
	gcc $(obj) -o $(target)

%.o:%.c
	$(CC) -c $< -o $@

#clean的target和终极目标的target不一样！！！
#每一个makefile的规则都有一个target
#在当前命令前加 - 表示如果这条命令执行失败，也会继续向下执行
.PHONY:clean
clean:
	-rm -rf $(obj) $(target)

hello:
	echo "hello, makefile"






###### END ######
