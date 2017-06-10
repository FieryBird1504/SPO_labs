#define BUFFER_SIZE 512

#ifdef _WIN32

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> 
		
#else 
	
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	

#endif 
 
struct Info;

struct Info *createInfoStruct();


void StartWork(struct Info *); 

void CreateNewProcess(struct Info *);

void CreateSem(struct Info *);
void Work(struct Info *);
void Close(struct Info *);

void CreateInfo(struct Info *);

void WaitForClient(struct Info *);

void WaitInput(struct Info *);
		
void Print(struct Info *);
