CC = gcc
CFLAGS = -c -Wall -pedantic
CLIENT = client
SERVER = server


ifeq ($(OS), Windows_NT)

STAND := -std=c99
postfix = win

else

STAND := -std=gnu99
postfix = lin

endif

ifeq ($(OS), Windows_NT)

all: $(CLIENT) $(SERVER) Write_cl Write_ser

else

all: $(CLIENT) $(SERVER)

endif

$(SERVER): server_$(postfix).o main.o
	$(CC) server_$(postfix).o main.o -o $(SERVER)

server_$(postfix).o: server_$(postfix).c
	$(CC) $(CFLAGS) $(STAND) server_$(postfix).c
	
$(CLIENT): client_$(postfix).o main.o
	$(CC) client_$(postfix).o main.o -o $(CLIENT)

client_$(postfix).o: client_$(postfix).c
	$(CC) $(CFLAGS) $(STAND) client_$(postfix).c

main.o: main.c
	$(CC) $(CFLAGS) $(STAND) main.c

# Windows only

Write_ser: Write_ser.o 
	$(CC) Write_ser.o -o Write_ser

Write_ser.o: Write_ser.c
	$(CC) $(CFLAGS) $(STAND) Write_ser.c	
	
Write_cl: Write_cl.o 
	$(CC) Write_cl.o -o Write_cl

Write_cl.o: Write_cl.c
	$(CC) $(CFLAGS) $(STAND) Write_cl.c

ifeq ($(OS), Windows_NT)

clean:
	del /f /q *.o
	del /f /q *.exe
	del /f /q *.dll

else

clean:
	rm -f *.o
	rm -f $(SERVER)
	rm -f $(CLIENT)
	
endif
