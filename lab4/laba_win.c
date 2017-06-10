#include "headers.h"
int lastThread;
BOOL flag = TRUE;

struct Data 
{
	HANDLE hand;
	CRITICAL_SECTION cs;
	int count;
};

struct Stack {
	
		HANDLE hand;
	
		struct Stack *next;
};

struct Stack *createStackStruct()
{
	return malloc(sizeof(struct Stack));	
}

struct Data *createDataStruct()
{
	return malloc(sizeof(struct Data));	
}

void CreateControlThread(struct Data *data)
{
	InitializeCriticalSection( &cs1 );
	data->count = 0;
}
void DelControlThread(struct Data *data)
{
	DeleteCriticalSection(&cs1);
}

void CreateNewThread(struct Data *data, struct Stack **stack)
{	
	data->count++;
	sizeh = data->count;
	HANDLE hThread;
	
	hThread = CreateThread(NULL, 0,MyThread,
		(LPVOID)data, 0, NULL);
	
	data->hand=hThread;
	push(*&stack, data);
}

DWORD WINAPI MyThread(LPVOID lpParameter)
  {
	struct Data data = *(struct Data *)lpParameter;
	
	while (TRUE)
	{
		EnterCriticalSection(&cs1);
		
			printf("%d___", data.count);
			printf("%d___", data.count);
			printf("%d___", data.count);
			printf("\n");
			LeaveCriticalSection( &cs1 );
		
		if (lastThread==data.count)
		{

			flag = TRUE;
			lastThread=0;
			ExitThread(0);
		}
		

	}
	
}	
  

void CloseThread(struct Stack **stack, struct Data *data, int code)
{
	if (code==0 && flag)
	{
		flag = FALSE;
		lastThread=data->count;
		data->count--;
		sizeh=data->count;
		pop(*&stack);
	}
	else if (code==1)
	{

		while (*stack)
		{
			TerminateThread((*stack)->hand, 0);
			pop(*&stack);
		}
	DeleteCriticalSection(&cs1);
	}
}

void push(struct Stack **head, struct Data *data) {

	struct Stack *tmp = (struct Stack *)malloc(sizeof(struct Stack));
	if (tmp == NULL)
		exit(1);
	tmp->next = *head;	
 
	tmp->hand = data->hand;	

	*head = tmp;
}

void pop(struct Stack **head) {

	struct Stack *out;

	if (*head == NULL)
		exit(1);

	out = *head;
	*head = (*head)->next;
	
	free(out);
}

