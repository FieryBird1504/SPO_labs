#ifndef HEADER_H
#define HEADER_H

#ifdef _WIN32

#include <windows.h>

#else

#include <pthread.h>

#endif

#include <locale.h>

#define BUFFER_SIZE 512
#define MESSAGE_SIZE 100

char buffer[BUFFER_SIZE];
char message[MESSAGE_SIZE];

struct Data
{
#ifdef _WIN32
	HANDLE hThreads[2];
	HANDLE hMutex;
#else
	pthread_t pWriter;
	pthread_t pReader;
	pthread_mutex_t pMutex;
#endif

	struct Stack *stack;
};

struct Stack
{
	char fileName[100];
	struct Stack *next;
};

void push(struct Stack **head, char name[]);
void pop(struct Stack **head);

void SearchFiles(struct Stack **stack);

void SendData(char *fileName, int typeOfMessage);

#ifdef _WIN32

__stdcall unsigned ThreadReader(void* fdata);
__stdcall unsigned ThreadWriter(void* fdata);

#else

void* ThreadReader(void *arg);
void* ThreadWriter(void *arg);

#endif

void CreateThreads(struct Data *data);

void CreateMutexForThreads(struct Data *data);

void CloseApp(struct Data *data);

void WaitThreads(struct Data *data);

void LoadLib();

#endif
