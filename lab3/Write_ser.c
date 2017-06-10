#include <stdlib.h>
#include <stdio.h>
#include <conio.h> 
#include <windows.h>
#define BUFFER_SIZE 512


int main(void)
{
	HANDLE hPipe;
	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe_c");
	
	hPipe = CreateNamedPipe( 
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
	

	if (hPipe == INVALID_HANDLE_VALUE) 
    {
	 printf("111CreatePipe failed: error code %d\n", (int)GetLastError());
        return 0;
    }
	

	
//	HANDLE hSemServer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemServer"));
	HANDLE hSemClient = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemClient"));

	
	if(!ConnectNamedPipe( 
		hPipe,NULL )) 
	{  
		CloseHandle(hPipe); 
		printf("ay///Press key to finish the server: ");
		_getch();
		return 0;
	}

	char str[BUFFER_SIZE]="";
	char ser[] = "server: ";

	DWORD dwWritten; 
 
	if (!WriteFile( hPipe, "server ready", BUFFER_SIZE, 
		&dwWritten, NULL))  
	{
		int Error = GetLastError();
		printf("%d\n",Error);
	}
	
	printf("OK\n");
	ReleaseSemaphore(hSemClient, 1, NULL);	

	while(strcmp(str, "--"))
	{ 

		fflush(stdin);
		gets(str);
			
		strcat(ser,str);
		DWORD dwWritten2; 
		if (!WriteFile( hPipe, ser, BUFFER_SIZE, 
			&dwWritten2, NULL))  
		{
			break;
		} 
		
		
		ReleaseSemaphore(hSemClient, 1, NULL);
		strcpy(ser,"server: ");	
	} 

	
	CloseHandle(hPipe);
 	printf("The end Write!");

	
	return 0;
	
}
