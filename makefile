CFLAGS = -Wall -ansi -g -O2
CC=gcc
filter : filter.o
clean:
	rm *.o *.jpg filter