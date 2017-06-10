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
	inf->segmentID = shmget(key, BUFFER_SIZE, 0666 | IPC_CREAT);	
	
	if (inf->segmentID == -1)
	{
		semctl(inf->semID, 0, IPC_RMID, 0);
		shmdt(inf->segmentPtr);
		perror("shmget");
		exit(0);
	}
	
	if (inf->segmentPtr == (char*)-1)
	{
		perror("shmat");
		exit(0);
	}
	inf->segmentPtr = shmat(inf->segmentID, NULL, 0);	
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

				semop(inf->semID, &(inf->semBuffClient), 1);
			}
		}	break;
		
		default:
		{}
			break;
	}
	
} 


void Close(struct Info *inf)
{
	
	semctl(inf->semID, 0, IPC_RMID, 0);
	shmdt(inf->segmentPtr);	
	return;
}


void CreateSem(struct Info *inf)
{
	int key = ftok("server.c", 's');  				//преобразование сущь. полное имя и целочисленный индикатор в значение типа key_t (ключ IPC)			
	inf->semID = semget(key, 2, 0666 | IPC_CREAT);
	semctl(inf->semID, 0, SETVAL, (int)0);
	semctl(inf->semID, 1, SETVAL, (int)0);
	inf->semBuffServer.sem_num = 0;
	inf->semBuffServer.sem_op = -1;
	inf->semBuffClient.sem_num = 1;
	inf->semBuffClient.sem_op = 1;
}

void WaitForClient(struct Info * inf)
{
	printf("Сервер ожидает подключения клиента.\n");	
	
	while (semctl(inf->semID, 0, GETVAL, 0) != 1);
	
	printf("Клиент подключён\n");
	
}
 
void WaitInput(struct Info * inf)
{
	semop(inf->semID, &(inf->semBuffServer), 1);

	while (semctl(inf->semID, 0, GETVAL, 0) != 1);
}
		
void Print(struct Info * inf)
{
	char str[BUFFER_SIZE];
	
	if (sprintf(str, "%s", inf->segmentPtr))
		{
			printf("user: ");
			printf("%s\n", str);
		}	
}
