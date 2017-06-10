#include "headers.h"

struct Data 
{
		pthread_mutex_t pMutex;
		pthread_t pThread;

		int count;
};

struct Stack {
		pthread_t pThread;	
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
	pthread_mutex_init(&(data->pMutex), NULL);
	data->count = 0;
}

void DelControlThread(struct Data *data)
{
	pthread_mutex_destroy(&(data->pMutex));
}

void* ThreadFun(void* fdata)
{
	struct Data data = *(struct Data *)fdata;
	free(fdata);
	while (1)
	{
		pthread_mutex_lock(&(data.pMutex)); 
		printf("%d___",data.count);
		printf("%d___",data.count);
		printf("%d___",data.count);
		printf("\n");
		pthread_mutex_unlock(&(data.pMutex));
	}
}

void CreateNewThread(struct Data *data, struct Stack **stack)
{
	data->count++;
	sizeh = data->count;
	struct Data *tempData = (struct Data *)malloc(sizeof(struct Data));
	*tempData = *data;
	pthread_create(&(tempData->pThread), NULL, &ThreadFun, tempData);
	push(stack, tempData);
}

void CloseThread(struct Stack **stack,struct Data *data, int code)
{
	if (code==0)
	{
		data->count--;
		sizeh = data->count;
		pthread_cancel((*stack)->pThread);
		pop(*&stack);
	}
	else if (code==1)
	{

		while(*stack)
		{
			pthread_cancel((*stack)->pThread);
			pop(*&stack);
			data->count--;
		}
		pthread_mutex_destroy(&(data->pMutex));
		system("clear");
	}
}

char _getch()
{
	struct termios old, new;
	char ch;
	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	ch = getchar();

	tcsetattr(0, TCSANOW, &old);
	return ch;
}

void push(struct Stack **head, struct Data *data) {

	struct Stack *tmp = (struct Stack *)malloc(sizeof(struct Stack));
	if (tmp == NULL)
		exit(1);
	tmp->next = *head;    
	
	tmp->pThread = data->pThread;
	
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

