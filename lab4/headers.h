#ifndef HEADERS_H
#define HEADERS_H

#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64) 

#include <windows.h>
#include <process.h> 
#include <conio.h> 

DWORD WINAPI MyThread(LPVOID lpParameter);
CRITICAL_SECTION cs1;

#else 

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

void* ThreadFun(void* fdata);
char _getch();

#endif 

int sizeh;

struct Data;

struct Stack;

void push(struct Stack **, struct Data *);
void pop(struct Stack **);

struct Data *createDataStruct();

void CreateNewThread(struct Data *, struct Stack **);
void CloseThread(struct Stack **, struct Data *, int code);
void DelControlThread(struct Data *);
void CreateControlThread(struct Data *);

#endif
