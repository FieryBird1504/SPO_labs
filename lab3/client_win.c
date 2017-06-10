#include "head.h"

struct Info
{
	HANDLE hPipe;
	
	HANDLE hSemServer;
	HANDLE hSemClient;
};

struct Info *createInfoStruct()
{
	return malloc(sizeof(struct Info));	
}

void StartWork(struct Info *inf)
{

	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe_c");
	inf->hPipe = CreateFile( 
         PipeName,	      // имя канала
         GENERIC_READ |  // чтение и запись в канал
         GENERIC_WRITE, 
         0,              // нет разделяемых операций 
         NULL,           // защита по умолчанию
         OPEN_EXISTING,  // открытие существующего канала 
         0,              // атрибуты по умолчанию
         NULL);          // нет дополнительных атрибутов 
	
	if (inf->hPipe == INVALID_HANDLE_VALUE) 
    {
	 printf("CreatePipe failed: error code %d\n", (int)GetLastError());
	 
        return ;
    }
	
}

void CreateNewProcess(struct Info *inf)
{
	STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	if (CreateProcess("Write_cl.exe",NULL,
		NULL,NULL,FALSE,0,NULL,NULL,&cif,&pi))
	{
		printf("Write_cl\n");
	}
	
} 

void Work( struct Info *inf)
{
	char str[BUFFER_SIZE]="";
	
	printf("Connected to server.\n");
	
	
	
	ReleaseSemaphore(inf->hSemServer, 1, NULL);

	

	while(strcmp(str, "--"))
	{ 

		WaitForSingleObject(inf->hSemClient, INFINITE);
		

		DWORD dwRead;
		if (ReadFile(inf->hPipe, str,    
		BUFFER_SIZE, &dwRead,NULL ))  
		{
			
			printf("%s\n", str);
		} 


	} 
	
}

void Close(struct Info *inf)
{
	
	CloseHandle(inf->hPipe);
	CloseHandle(inf->hSemServer);
	CloseHandle(inf->hSemClient);	
	printf("Press key to finish the server: ");
    _getch();	
	return;
}


void CreateSem(struct Info *inf)
{
	inf->hSemServer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemServer"));
	inf->hSemClient = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemClient"));


}

void WaitForClient(struct Info * inf)
{
	printf("Connected to server.\n");	
	ReleaseSemaphore(inf->hSemServer, 1, NULL);
	
}
 
void WaitInput(struct Info * inf)
{
	WaitForSingleObject(inf->hSemClient, INFINITE);
}
		
void Print(struct Info * inf)
{
	char str[BUFFER_SIZE]="";
	
	DWORD dwRead;
		if (ReadFile(inf->hPipe, str,    
		BUFFER_SIZE, &dwRead,NULL ))  
		{
			
			printf("%s\n", str);
		} 
}
