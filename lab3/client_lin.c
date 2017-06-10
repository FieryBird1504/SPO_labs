#include "head.h"

struct Info
{
	int segmentID;
	char* segmentPtr;
	int semID;
	struct sembuf semBuffServer;
	struct sembuf semBuffClient; 
};

struct Info *createInfoStruct()
{
	return malloc(sizeof(struct Info));	
}

void StartWork(struct Info *inf)
{
	inf->segmentPtr = NULL;
	
	int key = ftok("server.c", 'c');
	inf->segmentID = shmget(key, BUFFER_SIZE, 0666);
		
	if (inf->segmentID == -1)
	{
		perror("shmget");
		exit(0);
	}
	
	
	inf->segmentPtr = shmat(inf->segmentID, NULL, 0);
	
	if (inf->segmentPtr == (char*)-1)
	{
		perror("shmat");
		exit(0);
	}
}

void CreateNewProcess(struct Info *inf)
{
	char str[BUFFER_SIZE];
	
	switch(fork())
	{
		case -1:
			perror("fork error...");
			exit(1);
		case 0:	
		{				
			while (1)
			{				
				fflush(stdin);
				fgets(str, BUFFER_SIZE, stdin);
				int i = strlen(str) - 1;
				if (str[i] == '\n')
				{
					str[i] = '\0';
				}
				
				if (!sprintf(inf->segmentPtr, "%s", str))
				{	
					break;
				}

				semop(inf->semID, &(inf->semBuffServer), 1);
			}
		}	break;
		
		default:
		{}
			break;
	}
	
} 


void Close(struct Info *inf)
{
	
	semop(inf->semID, &(inf->semBuffClient), 1);		
	semop(inf->semID, &(inf->semBuffServer), 1);
	return;
}


void CreateSem(struct Info *inf)
{
	int key = ftok("server.c", 's');
	inf->semID = semget(key, 0, 0666);
	if (inf->semID == -1)
	{
		exit(0);
	}
	inf->semBuffServer.sem_num = 0;
	inf->semBuffServer.sem_op = 1;
	inf->semBuffClient.sem_num = 1;
	inf->semBuffClient.sem_op = -1;
}

void WaitForClient(struct Info * inf)
{
	printf("Подключение к серверу.\n");
	
	semop(inf->semID, &(inf->semBuffServer), 1);
	
}
 
void WaitInput(struct Info * inf)
{
	while (semctl(inf->semID, 1, GETVAL, 0) != 1);
}
		
void Print(struct Info * inf)
{
	char str[BUFFER_SIZE];
	
	if (sprintf(str, "%s", inf->segmentPtr))
		{
			printf("server: ");
			printf("%s\n", str);
		}
		
		semop(inf->semID, &(inf->semBuffClient), 1);	
}
