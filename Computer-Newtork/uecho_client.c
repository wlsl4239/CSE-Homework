#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char*argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;

	struct sockaddr_in serv_adr, rom_adr;

	if (argc != 3) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);	//socket 반환값 : 파일디스크립터.
	if (sock == -1)
		error_handling("sock() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_adr = intet_addr(argv[1]);
	serv_adr.sin_port = htos(atoi(argv[2]));

	//connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))	//UDP소켓은 주소를 안갖고있는데 주소를 부여해줘서 // UDP에서의 connect는 option이다. (TCP는 필수)
	while (1)
	{

		fputs("Input message(Q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		//==write(sock, message, strlen(message));	sendto대신 write로 쓸수 있게 해준다.(sendto의 인자가 너무 많기때문에 쓰기 편하게 하기 위함)
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, mesage, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
		//==str_len = read(sock, mesage, BUF_SIZE);	recvfrom대신 read로 쓸수 있게 해준다.(recvfrom의 인자가 너무 많기때문에 쓰기 편하게 하기 위함)
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}

	//UDP는 데이터의 경계가 존재하기 때문에 
	//한번의 recvfrom함수호출을 통해서 하나의 메시지를 완전히 읽어들인다.
	//그리고 sendto 함수호출 시 IP와 PORT번호가 자동으로 할당되기 때문에
	//일반적으로 UDP의 클라이언트 프로그램에서는 주소정보를 할당하는 별도의 과정이 불필요하다.
	close(sock);
	return 0;
}

	}


}

void error_handling(char*message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}