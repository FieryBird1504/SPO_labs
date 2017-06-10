#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <aio.h>

#define BUFFER_SIZE 512

void WriteDataInFile(int fd, char* buff)
{
	struct aiocb aiocb;
	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = buff;
	aiocb.aio_nbytes = strlen(buff);
	aiocb.aio_lio_opcode = LIO_WRITE;	
	if (aio_write(&aiocb) == -1)
	{
		printf("Error at aio_write()\n");
		close(fd);
		exit(2);
	}
	while (aio_error(&aiocb) == EINPROGRESS);
}

void ReadDataFromFile(int fd, char* buff)
{
	memset(buff, 0, BUFFER_SIZE * sizeof(char));
	struct aiocb aiocb;
	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = buff;
	aiocb.aio_nbytes = BUFFER_SIZE;
	aiocb.aio_lio_opcode = LIO_READ;	
	if (aio_read(&aiocb) == -1)
	{
		printf("Error at aio_read()\n");
		close(fd);
		exit(2);
	}
	while (aio_error(&aiocb) == EINPROGRESS);
}