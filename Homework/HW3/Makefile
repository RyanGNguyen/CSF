# CSF Assignment 3 Makefile
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu++17

C_SRCS = csimmain.cpp csimfuncs.cpp 
C_OBJS = csimmain.o csimfuncs.o

%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

clean :
	rm -f $(EXECUTABLE) $(C_OBJS)
