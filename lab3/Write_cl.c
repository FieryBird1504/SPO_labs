#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> 
#define BUFFER_SIZE 512

int main(void)
{
	HANDLE hPipe;
	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe_s");
	hPipe = CreateFile( 
         PipeName,	      // имя канала
         GENERIC_READ |  // чтение и запись в канал
         GENERIC_WRITE, 
         0,              // нет разделяемых операций 
         NULL,           // защита по умолчанию
         OPEN_EXISTING,  // открытие существующего канала 
         0,              // атрибуты по умолчанию
         NULL);          // нет дополнительных атрибутов 
	
		if (hPipe == INVALID_HANDLE_VALUE) 
    {
	 printf("111CreatePipe failed: error code %d\n", (int)GetLastError());
        return 0;
    }
	
	HANDLE hSemServer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemServer"));
//	HANDLE hSemClient = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemClient"));


	char temp[BUFFER_SIZE]="";
	char user[] = "user: ";

	while(strcmp(temp, "--"))
	{ 

		fflush(stdin);
		gets(temp);
			
		strcat(user,temp);
		DWORD dwWritten3; 
		if (!WriteFile( hPipe, user, BUFFER_SIZE, 
			&dwWritten3, NULL))  
		{
			break;
		} 
		ReleaseSemaphore(hSemServer, 1, NULL);
		strcpy(user,"user: ");	
	} 
 
	CloseHandle(hPipe);
 //CloseHandle(hSemClient); 

 	printf("The end Write!");
	
	return 0;
}
