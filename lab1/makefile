CC = gcc
CFLAGS = -c -Wall -pedantic

STAND := -std=c99

name = lab1

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
	rm -f timeadd
	
endif

output: main.o lab_$(postfix).o timeadd
	$(CC) main.o lab_$(postfix).o -o $(name)
	
main.o: main.c
	$(CC) $(CFLAGS) main.c
		
lab_$(postfix).o:lab_$(postfix).c
	$(CC) $(CFLAGS) lab_$(postfix).c

timeadd: timeadd.o
	$(CC) timeadd.o -o timeadd

timeadd.o:
	$(CC) $(CFLAGS) timeadd.c
