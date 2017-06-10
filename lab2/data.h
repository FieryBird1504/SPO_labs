#ifndef DATA_H
#define DATA_H

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

#else

#include <sys/sem.h>

#endif

struct Data 
{
#if defined(_WIN32) || defined(_WIN64)
	PROCESS_INFORMATION prInfo;
#else
	pid_t pid;	
	key_t key;
	int semid;
	struct sembuf mybuff, mybuff1;
#endif
};

#endif //DATA_H
