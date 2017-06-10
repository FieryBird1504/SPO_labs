#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <locale.h>

#include "header.h"

typedef void (*ReadDataFromFile)(HANDLE, char*);
typedef void (*WriteDataInFile)(HANDLE, char*);

HINSTANCE library;

void push(struct Stack **head, char name[])
{
	struct Stack *tmp = (struct Stack *)malloc(sizeof(struct Stack));
	if (tmp == NULL)
	{
		exit(1);
	}
	tmp->next = *head;
	strcpy(tmp->fileName, ".\\Files\\");
	strcat(tmp->fileName, name);
	*head = tmp;
}

void pop(struct Stack **head) 
{

	struct Stack *out;
	if (*head == NULL)
	{
		exit(1);
	}
	out = *head;
	*head = (*head)->next;
	free(out);
}

void SearchFiles(struct Stack **stack)
{
	WIN32_FIND_DATA wfd;
 
    HANDLE hFind = FindFirstFile(".\\Files\\*.txt", &wfd);
	setlocale(LC_ALL, "");
 
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
			if (!strcmp(wfd.cFileName, "All files.txt")) 
			{
				continue;
			}
			push(stack, wfd.cFileName);
		} while (FindNextFile(hFind, &wfd) != 0);
 
        FindClose(hFind);
    }
}

void SendData(char *fileName, int typeOfMessage)
{
	if (typeOfMessage == 1)
	{
		strcpy(message, "Reader: Content of the file '");
		strcat(message, fileName);
		strcat(message, "' is read and is ready to record\n");
	}
	else
	{
		strcpy(message, "Writer: Content of the file '");
		strcat(message, fileName);
		strcat(message, "' is write in output file\n");
	}
}

__stdcall unsigned ThreadReader(void* fdata)
{
	struct Data *data = (struct Data *)fdata;
	ReadDataFromFile readDataFromFile = (ReadDataFromFile) GetProcAddress(library,"ReadDataFromFile");
	
	while (1)
	{
		WaitForSingleObject(data->hMutex, INFINITE);
		puts(message);
		if (data->stack == NULL)
		{
			break;
		}
		HANDLE hFile = CreateFile(data->stack->fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		(*readDataFromFile)(hFile, buffer);
		SendData(data->stack->fileName, 1);
		CloseHandle(hFile);
		ReleaseMutex(data->hMutex);
	}
	
	_endthreadex(0);
	return 0;
}

__stdcall unsigned ThreadWriter(void* fdata)
{		
	struct Data *data = (struct Data *)fdata;
	WaitForSingleObject(data->hMutex, INFINITE);
	HANDLE hFile = CreateFile(".\\Files\\All files.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	WriteDataInFile writeDataInFile = (WriteDataInFile) GetProcAddress(library,"WriteDataInFile");
	ReleaseMutex(data->hMutex);
	
	do
	{
		WaitForSingleObject(data->hMutex, INFINITE);
		puts(message);
		(*writeDataInFile)(hFile, buffer);
		SendData(data->stack->fileName, 0);
		pop(&(data->stack));
		ReleaseMutex(data->hMutex);
	} while (data->stack != NULL);

	CloseHandle(hFile);
	_endthreadex(0);

	return 0;
}

void CreateThreads(struct Data *data)
{
	data->hThreads[0]  = (HANDLE) _beginthreadex(NULL, 0, ThreadWriter, (void*) data, 0, NULL);
	data->hThreads[1]  = (HANDLE) _beginthreadex(NULL, 0, ThreadReader, (void*) data, 0, NULL);
}

void CreateMutexForThreads(struct Data *data)
{
	data->hMutex = CreateMutex(NULL, FALSE, NULL);
}

void CloseApp(struct Data *data)
{
	CloseHandle(data->hThreads[0]);
	CloseHandle(data->hThreads[1]);
	CloseHandle(data->hMutex);
	FreeLibrary(library);
}

void WaitThreads(struct Data *data)
{
	WaitForMultipleObjects(2, data->hThreads, TRUE, INFINITE);
}

void LoadLib()
{
	library = LoadLibrary("myfun.dll");
}
