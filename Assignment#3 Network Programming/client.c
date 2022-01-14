// group names
// bilal alqurneh  125955
// Abdalazez Irsheidat 126469
// Ali Alomari 134223
#include <sys/socket.h>
#include <netinet/in.h>	
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#define MAX 256
#define SA struct sockaddr
void handle_connection(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("> ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
    }
}
   
int main(int argc, char const *argv[]){

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    if(argc != 3 ){
        printf("please provide host and port number as an argument\n");
        exit(-1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));
   
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
    handle_connection(sockfd);
   
    close(sockfd);
}
