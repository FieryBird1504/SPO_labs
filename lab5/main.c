#include <stdio.h>

#include "header.h"

int main()
{
	struct Data data;
	struct Stack *stack = NULL;
	printf("\nWaiting...\n");
	SearchFiles(&stack);
	data.stack = stack;
	CreateMutexForThreads(&data);

	LoadLib();

	CreateThreads(&data);
	WaitThreads(&data);
	CloseApp(&data);
	
	printf("Press \"Enter\" key...\n");
	getchar();

	return 0;
}
