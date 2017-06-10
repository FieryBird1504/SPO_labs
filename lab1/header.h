#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
	
#include <windows.h>

#endif

struct Info
{
#if defined(_WIN32) || defined(_WIN64)
    STARTUPINFO stInfo;
    PROCESS_INFORMATION prInfo;
#else
    pid_t pid;
#endif
};

int createProc(struct Info*);

#ifndef _WIN32
    
void deleteProc(struct Info*);
    
#endif


#endif
