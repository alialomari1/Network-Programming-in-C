// Ali Alomari 134223

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#define MAX 256
#define SA struct sockaddr
   
void handle_connection(int sockfd)
{
    char buff[MAX];

    for (;;) {
        bzero(buff, MAX);
   
        read(sockfd, buff, sizeof(buff));
        printf("From client: %s\t", buff);
        
        for (int i = 0 ; i < strlen(buff);i++){
            buff[i] = toupper(buff[i]);
        }
        write(sockfd, buff, sizeof(buff));   
    }
}

int main(int argc, char const *argv[]){
     
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    if(argc != 2 ){
        printf("please provide a port number as an argument\n");
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
   
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
    pid_t pid;
    for (;;){
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("server accept failed...\n");
            exit(0);
        }
        else
            printf("server accept the client...\n");
        
        if((pid = fork()) == 0){
            close(sockfd);
            handle_connection(connfd);
            close(connfd);
            exit(0);
        }else{
            close(connfd);
        }
        
    }
    close(sockfd);
}
