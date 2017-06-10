CC = gcc
CFLAGS = -c -Wall -pedantic
STAND := -std=c99
TARGET = lab4

ifeq ($(OS), Windows_NT)

postfix = win
UNIXFLAG = 
THREADFLAG = 

else

postfix = linux

UNIXFLAG = -D_XOPEN_SOURCE=600
THREADFLAG = -pthread
	
endif

SOURCES = main.c laba_$(postfix).c
OBJECTS = $(SOURCES:.c=.o)


all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(THREADFLAG) $(OBJECTS) $(STAND) -o $@

.c.o:
	$(CC) $(THREADFLAG) $(CFLAGS) $(UNIXFLAG) $(STAND) $< -o $@


ifeq ($(OS), Windows_NT)

clean:
	del /f /q *.o
	del /f /q *.exe
	del /f /q *.dll

else

clean:
	rm -f *.o
	rm -rf $(TARGET)
	rm -rf $(OBJECTS)
	
endif
