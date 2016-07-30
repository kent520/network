#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

typedef unsigned char u8;

int tcp_cli(u8 cmd[])
{
	int sockfd, nwrite, nread;
	u8 recv_buf[200] = {0};
	struct sockaddr_in serv_addr;
	struct hostent* server;

	printf("----START--------------------------------------------------\n");

	// socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		goto exit;
	}

	// server = gethostbyname("192.168.77.222");
	server = gethostbyname("192.168.1.1");
	if (NULL == server) {
		perror("gethostbyname");
		goto exit;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char*)&(serv_addr.sin_addr.s_addr), (char*)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(5001);

	// connect
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		goto exit;
	}

	printf(">>> %s %d : cmd = %s\n", __func__, __LINE__, cmd);
	strcpy(cmd, "{\"type\":\"request_version\"}");
	printf(">>> %s %d : cmd = %s\n", __func__, __LINE__, cmd);

	nwrite = write(sockfd, cmd, strlen(cmd));
	if (nwrite < 0) {
		perror("write");
		goto exit;
	}

	// recv response
	memset (recv_buf, 0, sizeof(recv_buf));
	nread = read(sockfd, recv_buf, sizeof(recv_buf) - 1);
	if (0 == nread) {
		printf("server has closed.\n");
		close(sockfd);
		goto exit;
	} else if (nread < 0) {
		perror("read");
		goto exit;
	}

	printf(">>> result : %s\n", recv_buf);
	printf("----END--------------------------------------------------\n");

	close(sockfd);
	return 0;
exit:
	close(sockfd);
	return -1;
}

int main()
{
	u8 s[200] = "helloworld";
	tcp_cli(s);
	return 0;
}
