#declare the variable
CC = g++

CFLAGS=-c -Wall

all: ahencode ahdecode

ahencode: ahencode.o
	$(CC) ahencode.o -o ahencode

ahdecode: ahdecode.o
	$(CC) ahdecode.o -o ahdecode

ahencode.o:
	$(CC) $(CFLAGS) ahencode.cpp

ahdecode.o:
	$(CC) $(CFLAGS) ahdecode.cpp

.PHONY: clean
clean: 
	rm -rf *o ahencode ahdecode
