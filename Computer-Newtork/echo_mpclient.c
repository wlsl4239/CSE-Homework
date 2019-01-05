#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);


int main(int argc, char*argv[])
{
	int sock;
//	pid_t pid;
	fd_set temp;
	fd_set temp_cpy;
	struct timeval timeout;
	int fd_max,fd_num;

	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");
		
	FD_ZERO(&temp);
    FD_SET(sock, &temp);

    FD_SET(0, &temp);
	fd_max = sock;

	while (1)
	{
        temp_cpy = temp;
        
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		//시간 설정
		//fd값 복사

		if ((fd_num = select(fd_max + 1, &temp_cpy, 0, 0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;

		if (FD_ISSET(0,&temp_cpy)) //사용자로부터 입력(fd 0번)의 변화 -> 사용자로부터 메세지를 입력받음.
			write_routine(sock,buf);
		if (FD_ISSET(sock,&temp_cpy)) //sock의 변화 -> server로부터 메세지를 받음.
			read_routine(sock,buf);
	}
/*	pid = fork();

	if (pid == 0)
		write_routine(sock, buf);
	
	else
		read_routine(sock, buf);
	*/
	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
//	while (1)
//	{
		int str_len = read(sock, buf, BUF_SIZE);
		if (str_len == 0)
			return;

		buf[str_len] = 0;
		printf("Message from server : %s", buf);
//	}
}
void write_routine(int sock, char *buf)
{
//	while (1)
//	{
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
//	}
}
	void error_handling(char*message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
