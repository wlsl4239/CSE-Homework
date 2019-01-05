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

	sock = socket(PF_INET, SOCK_DGRAM, 0);	//socket ��ȯ�� : ���ϵ�ũ����.
	if (sock == -1)
		error_handling("sock() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_adr = intet_addr(argv[1]);
	serv_adr.sin_port = htos(atoi(argv[2]));

	//connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))	//UDP������ �ּҸ� �Ȱ����ִµ� �ּҸ� �ο����༭ // UDP������ connect�� option�̴�. (TCP�� �ʼ�)
	while (1)
	{

		fputs("Input message(Q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		//==write(sock, message, strlen(message));	sendto��� write�� ���� �ְ� ���ش�.(sendto�� ���ڰ� �ʹ� ���⶧���� ���� ���ϰ� �ϱ� ����)
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, mesage, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
		//==str_len = read(sock, mesage, BUF_SIZE);	recvfrom��� read�� ���� �ְ� ���ش�.(recvfrom�� ���ڰ� �ʹ� ���⶧���� ���� ���ϰ� �ϱ� ����)
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}

	//UDP�� �������� ��谡 �����ϱ� ������ 
	//�ѹ��� recvfrom�Լ�ȣ���� ���ؼ� �ϳ��� �޽����� ������ �о���δ�.
	//�׸��� sendto �Լ�ȣ�� �� IP�� PORT��ȣ�� �ڵ����� �Ҵ�Ǳ� ������
	//�Ϲ������� UDP�� Ŭ���̾�Ʈ ���α׷������� �ּ������� �Ҵ��ϴ� ������ ������ ���ʿ��ϴ�.
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