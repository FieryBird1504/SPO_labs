#include "headers.h"

int main(int argc, char *argv[])
{
	struct Stack *stack = NULL;
	struct Data *data;
	
	data = createDataStruct();

		CreateControlThread(data);
		
		while (1)
		{
			switch (_getch())
			{
			case '+':
				CreateNewThread(data, &stack);
				break;
			case '-':
				if (stack){
					CloseThread(&stack, data,0);
					}
				break;	
			case 'q':
					CloseThread(&stack, data,1);
				return 0;
				break;
			}
		}

		DelControlThread(data);
		free(stack);
	return 0;
}
