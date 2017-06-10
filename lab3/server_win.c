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

	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe_s");

	inf->hPipe = CreateNamedPipe( 
          PipeName,			   // имя канала
          PIPE_ACCESS_DUPLEX,       // чтение и запись из канала
          PIPE_TYPE_MESSAGE |       // передача сообщений по каналу
          PIPE_READMODE_MESSAGE |   // режим чтения сообщений 
          PIPE_WAIT,                // синхронная передача сообщений 
          PIPE_UNLIMITED_INSTANCES, // число экземпляров канала 
          4096,			   // размер выходного буфера
          4096,		          // размер входного буфера  
          NMPWAIT_USE_DEFAULT_WAIT, // тайм-аут клиента 
          NULL);                    // защита по умолчанию
	
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
	if (CreateProcess("Write_ser.exe",NULL,
		NULL,NULL,FALSE,0,NULL,NULL,&cif,&pi))
	{
		printf("Write\n");
	}
	
} 

void Work( struct Info *inf)
{
	char temp[BUFFER_SIZE]="";
	
	while(strcmp(temp, "--")) 
	{
		
		WaitForSingleObject(inf->hSemServer, INFINITE);

			DWORD dwRead1;
			if (ReadFile( inf->hPipe, temp, BUFFER_SIZE, 
				&dwRead1, NULL))  
			{
				printf("%s\n", temp);
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
	inf->hSemServer = CreateSemaphore(NULL, 0, 1, "SemServer");
	inf->hSemClient = CreateSemaphore(NULL, 0, 1, "SemClient");	
}

void WaitForClient(struct Info * inf)
{
	printf("The server is waiting for connection with a client.\n");
	
	if(!ConnectNamedPipe( 
		inf->hPipe,NULL )) 
	{  
		printf("Connect: error code %d\n", (int)GetLastError());
		CloseHandle(inf->hPipe); 
		printf("Press key to finish the server: ");
		_getch();

		return ;
	}	
	WaitForSingleObject(inf->hSemServer, INFINITE);
	printf("Client connected\n");
	
}
 
void WaitInput(struct Info * inf)
{
	WaitForSingleObject(inf->hSemServer, INFINITE);
}
		
void Print(struct Info * inf)
{
	char temp[BUFFER_SIZE]="";
	
	DWORD dwRead1;
	if (ReadFile( inf->hPipe, temp, BUFFER_SIZE, 
		&dwRead1, NULL))  
	{
		printf("%s\n", temp);
	} 		
}
