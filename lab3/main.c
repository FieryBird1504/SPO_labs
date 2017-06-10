#include "head.h"

int main()
{	
	struct Info *info;
	info = createInfoStruct();

	StartWork(info);
	
	CreateSem(info);

	CreateNewProcess(info);

	WaitForClient(info);
	
	while(1) 
	{
		
		WaitInput(info);
		
		Print(info);			

	}

	Close(info);	
	
	return 0;
}

