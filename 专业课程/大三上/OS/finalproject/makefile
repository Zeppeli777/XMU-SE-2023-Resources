# Operating Systems Project
# Makefile for myshell

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = myshell
OBJS = myshell.o utility.o

.PHONY: all clean

all: $(TARGET)

# Build myshell executable
myshell: myshell.o utility.o myshell.h
	$(CC) $(CFLAGS) myshell.o utility.o -o myshell

# Compile myshell.c
myshell.o: myshell.c myshell.h
	$(CC) $(CFLAGS) -c myshell.c

# Compile utility.c
utility.o: utility.c myshell.h
	$(CC) $(CFLAGS) -c utility.c

# Clean up compiled files
clean:
	rm -f $(TARGET) $(OBJS)
