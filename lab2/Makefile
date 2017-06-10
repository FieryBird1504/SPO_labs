CC = gcc
CFLAGS = -c -Wall -pedantic
STAND:=  -std=gnu99

name = lab2

all: output

ifeq ($(OS), Windows_NT)

postfix = win

clean:
	del /f /q *.o
	del /f /q *.exe

else

postfix = linux

clean:
	rm -f *.o
	rm -f $(name)
	
endif

output: headers.o stack.o main.o 
	$(CC) headers.o stack.o main.o -o $(name)

stack.o: stack.c
	$(CC) $(CFLAGS) $(STAND) stack.c 
	
main.o: main.c
	$(CC) $(CFLAGS) $(STAND) main.c 

headers.o: headers_$(postfix).c
	$(CC) $(CFLAGS) $(STAND) headers_$(postfix).c -o headers.o
