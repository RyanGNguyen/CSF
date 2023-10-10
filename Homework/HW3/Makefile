# CSF Assignment 3 Makefile
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu++17 -lstdc++

C_SRCS = csimmain.cpp csimfuncs.cpp 
C_OBJS = csimmain.o csimfuncs.o
EXECUTABLE = csim

all : $(EXECUTABLE)

$(EXECUTABLE) : $(C_OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(C_OBJS)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

clean :
	rm -f $(EXECUTABLE) $(C_OBJS)
