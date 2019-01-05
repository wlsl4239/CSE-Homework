#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads);	//0 : stdin

	/*
	timeout.tv_sec = 5;
	time.tv_usec = 5000;
	*/

	while (1)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;		//select하기 전에 timeval값을 초기화시켜줘야한다.

		result = select(1, &temps, 0, 0, &timeout);
		if (result == -1)
		{
			puts("select() error!");
			break;
		}
		else if (result == 0)
		{
			puts("Time-out!");
		}
		else
		{
			if (FD_ISSET(0, &temps))
			{
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console : %s", buf);
			}
		}
	}
	return 0;
}

