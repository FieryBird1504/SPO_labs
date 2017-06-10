#ifndef HEADERS_H
#define HEADERS_H

#include <stdlib.h>
#include <stdio.h>


#if defined(_WIN32) || defined(_WIN64) 

#include <windows.h> 
#include <conio.h> 

#else 

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

char _getch();

#endif 

#include "stack.h"

void CreateNewProcess(char *, struct Data *, struct Stack **);
void CloseProcess(struct Stack **, struct Data *, int code);

void CreateSignal(struct Data *);
void PrintProcesses(char **argv, struct Data *);

#endif //HEADERS_H
