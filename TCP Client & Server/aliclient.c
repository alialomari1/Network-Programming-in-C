#include	<sys/socket.h>
#include	<netinet/in.h>	
#include   	<arpa/inet.h>
#include 	<netinet/in.h>
#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<unistd.h>

#define	MAXLINE	8192

void str_cli(FILE * fp, int sockfd , long w) {
	
	int n;	
	int c=0;
	char sendline[MAXLINE], rcvline[MAXLINE];
		
	while (fgets(sendline, MAXLINE, fp) != NULL) {c++;
	
		printf("Input#%d: %s\n", c , sendline);
		n = write(sockfd, sendline, strlen(sendline));
		if (n < 0) perror("str_cli: write error"); 
		
		n = read(sockfd, rcvline, MAXLINE);
		if (n <= 0) perror("str_cli: read error");
		rcvline[n] = 0;
		
		printf("The result#%d is: " , c );
		fputs(rcvline, stdout);
		printf("\n\n\n");
		 usleep(w); 
	}
}

int main(int argc, char ** argv) {

	int sockfd, c_retVal;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) perror("main: socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(atoi(argv[2])); 

	c_retVal = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (c_retVal < 0) {perror("main: connect error");return -1;}
	
	FILE* fp = fopen(argv[4],"r");
	 if(fp == NULL){ perror("main:open file error");return -1;}
	 long w = atoi(argv[3]);
	str_cli(fp , sockfd , w);
	
	return 0;
}