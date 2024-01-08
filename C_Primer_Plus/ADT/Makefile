# Makefile for films program

# 编译器
CC = gcc

# 编译选项
CFLAGS = -I.

# 源文件
SRCS = films.c list.c

# 目标文件
OBJS = $(SRCS:.c=.o)

# 可执行文件
TARGET = films

# 编译规则
all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
