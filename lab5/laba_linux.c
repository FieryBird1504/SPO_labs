#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <string.h>
#include <aio.h>
#include <dlfcn.h>

#include "header.h"

volatile int running_threads = 0;

void *library;

void push(struct Stack **head, char name[])
{
	struct Stack *tmp = (struct Stack *)malloc(sizeof(struct Stack));
	if (tmp == NULL)
	{
		exit(1);
	}
	tmp->next = *head;
	strcpy(tmp->fileName, "./Files/");
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
	DIR *dfd;
	struct dirent *dp;
	dfd = opendir("./Files/");
 
    while((dp = readdir(dfd)) != NULL)
	{
		size_t len = strlen(dp->d_name);
		if (!strcmp(dp->d_name, "All files.txt")) 
		{
			continue;
		}
		else if (len > 4 && strcmp(dp->d_name + len - 4, ".txt") == 0)
		{
			push(stack, dp->d_name);
		}
	}
 
    closedir(dfd);
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

void* ThreadReader(void *arg)
{
	struct Data *data = (struct Data *)arg;
	void (*readDataFromFile)(int, char*);
	*(void **) (&readDataFromFile) = dlsym(library, "ReadDataFromFile");
	
	while (1)
	{
		pthread_mutex_lock(&(data->pMutex));
		puts(message);
		if (data->stack == NULL)
		{
			break;
		}
		int fd = open(data->stack->fileName, O_RDONLY);
		(*readDataFromFile)(fd, buffer);
		SendData(data->stack->fileName, 1);
		close(fd);
		pthread_mutex_unlock(&(data->pMutex));
		usleep(1);
	}
	
	running_threads--;
	return 0;
}

void* ThreadWriter(void *arg)
{
	struct Data *data = (struct Data *)arg;
	pthread_mutex_lock(&(data->pMutex));
	int fd = open("./Files/All files.txt", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND);
	void (*writeDataInFile)(int, char*);
	*(void **) (&writeDataInFile) = dlsym(library, "WriteDataInFile");
	pthread_mutex_unlock(&(data->pMutex));
	usleep(1);
	
	do
	{
		pthread_mutex_lock(&(data->pMutex));
		puts(message);
		(*writeDataInFile)(fd, buffer);
		SendData(data->stack->fileName, 0);
		pop(&(data->stack));
		pthread_mutex_unlock(&(data->pMutex));
		usleep(1);
	} while (data->stack != NULL);
	
	close(fd);
	running_threads--;
	return 0;
}

void CreateThreads(struct Data *data)
{
	pthread_create(&(data->pReader), NULL, &ThreadReader, data);
	running_threads++;
	pthread_create(&(data->pWriter), NULL, &ThreadWriter, data);
	running_threads++;
}

void CreateMutexForThreads(struct Data *data)
{
	pthread_mutex_init(&(data->pMutex), NULL);
}

void CloseApp(struct Data *data)
{
	pthread_mutex_destroy(&(data->pMutex));
	dlclose(library);
}

void WaitThreads(struct Data *data)
{
	while (running_threads > 0)
	{
		usleep(1);
	}
}

void LoadLib()
{
	library = dlopen("./myfun.so", RTLD_LAZY);
}
