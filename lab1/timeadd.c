#include <stdio.h>
#include <time.h>
int main()
{
    time_t t;
    time(&t);
   	printf( "The time is %s\nPress \"Enter\" to contitue\n", ctime(&t));
	getchar();
    return 1;
}
