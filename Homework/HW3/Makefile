# CSF Assignment 3 Makefile
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu++17

C_SRCS = csimmain.cpp csimfuncs.cpp 
C_OBJS = csimmain.o csimfuncs.o

%.o : %.c
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

csim : $(C_OBJS)
	$(CC) -o $@ $(C_OBJS)

clean :
	rm -f *.o
