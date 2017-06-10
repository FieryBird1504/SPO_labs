CC = gcc
SHAREDFLAG = -shared
STAND:= -std=c99
APP = lab5

ifeq ($(OS), Windows_NT)

postfix = win
CFLAGS = -c -Wall -pedantic
LIB = myfun.dll

else

postfix = linux
CFLAGS = -c -Wall -pedantic -fPIC -D_XOPEN_SOURCE=600
LIB = myfun.so

THREADFLAG = -pthread
AIOFLAG = -lrt
LIBFLAG = -ldl
	
endif

APP_SOURCES = laba_$(postfix).c main.c
APP_OBJECTS = $(APP_SOURCES:.c=.o)
LIB_SOURCES = fun_$(postfix).c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)

all: $(LIB) $(APP)

ifeq ($(OS), Windows_NT)

$(LIB): $(LIB_OBJECTS)
	$(CC) $(LIB_OBJECTS) $(SHAREDFLAG) $(STAND) -o $@

$(APP): $(APP_OBJECTS)
	$(CC) $(APP_OBJECTS) $(STAND) -o $@
	
.c.o:
	$(CC) $(CFLAGS) $(STAND) $< -o $@
	 
else


$(LIB): $(LIB_OBJECTS)
	$(CC) $(LIB_OBJECTS) $(SHAREDFLAG) $(STAND) -o $@ $(AIOFLAG)

$(APP): $(APP_OBJECTS)
	$(CC) $(THREADFLAG) $(APP_OBJECTS) $(STAND) -o $@ $(LIBFLAG)
	
.c.o:
	$(CC) $(THREADFLAG) $(CFLAGS) $(STAND) $< -o $@ $(AIOFLAG) $(LIBFLAG)

endif


ifeq ($(OS), Windows_NT)

clean:
	del /f /q *.o
	del /f /q *.exe
	del /f /q *.dll

else

clean:
	rm -f *.o
	rm -f $(APP)
	rm -f $(LIB)
	
endif
