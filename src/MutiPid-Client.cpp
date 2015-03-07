//============================================================================
// Name        : MutiPid-Client.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

#define PORT 1234
#define MAXDATASIZE 100

void process(FILE *fp, int sockfd);
char* getMessage(char* sendline, int len, FILE* fp);

int main(int argc, char *argv[]) {
	int sockfd;
	struct hostent *he;
	struct sockaddr_in server;

	if (argc != 2) {
		printf("ip%s\n", argv[1]);
		exit(1);
	}

	if ((he = gethostbyname(argv[1])) == NULL) {
		printf("gethostbyname() error");
		exit(1);
	} else {
		printf("hostname:%s", he->h_name);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket() error.");
		exit(1);
	}

	bzero(&server, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr *) he->h_addr);
	cout << endl << server.sin_addr.s_addr << endl;

	if (connect(sockfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("connect() error.");
		exit(1);
	}

	process(stdin, sockfd);

	close(sockfd);

}

void process(FILE *fp, int sockfd) {
	char sendline[MAXDATASIZE], recvline[MAXDATASIZE];
	int num;
	printf("connected!\n");
	printf("input name:");
	if (fgets(sendline, MAXDATASIZE, fp) == NULL) {
		printf("\n QUIT\n");
		return;
	}
	printf("client name len:%d\n",strlen(sendline));
	send(sockfd, sendline, strlen(sendline), 0);
	while (getMessage(sendline, MAXDATASIZE, fp) != NULL) {
		printf("msg len:%d\n",strlen(sendline));
		send(sockfd, sendline, strlen(sendline), 0);
		if ((num = recv(sockfd, recvline, MAXDATASIZE, 0)) == 0) {
			printf("Server termiated\n");
			return;
		}
		recvline[num] = '\0';
		printf("Server MSG:%s\n", recvline);
	}
	printf("\nExit\n");
}

char* getMessage(char* sendline, int len, FILE* fp) {
	printf("Input msg:");
	return (fgets(sendline, MAXDATASIZE, fp));
}

