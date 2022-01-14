// group names :
// Ali Alomari 134223
// bilal alqurneh  125955
// Abdalazez Irsheidat 126469
#include	<sys/socket.h>
#include	<netinet/in.h>	
#include   	<arpa/inet.h>
#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<unistd.h>
#include	<sys/types.h>
#include	<time.h>

#define	MAXLINE	8192
void myfunction(char msg[]  , int *characters, int *words){
int counter=0;
  for(int i=0 ; i< *characters ; i++)
  if(msg[i] == ' ') counter++;
  
  *words = counter+1;
  (*characters)--;
};

void dg_echo(int sockfd, struct sockaddr * pcliaddr,socklen_t clilen) 
{
	
	ssize_t n,m;
	socklen_t len;
	char msg[MAXLINE];
	char str[MAXLINE];
	for ( ; ; )
	{	int wc=0 , cc=0; // wc :words counter . cc : characters counter .

		len = clilen;
		n=recvfrom(sockfd, msg, MAXLINE, 0, (struct sockaddr *) pcliaddr, &len);
		if(n < 0) perror("recvfrom error");
		msg[n]=0;
    cc=n;
    myfunction(msg , &cc , &wc);
if(cc==0) wc =0 ;
    char * ip= inet_ntoa(((struct sockaddr_in * ) pcliaddr)->sin_addr); // ip of the client.

    int port =htons(((struct sockaddr_in * ) pcliaddr)->sin_port);  // port number of the client .
    
		printf("Received text is: %s(received packet from: %s port %d)\n\n\n", msg , ip , port);
		
 snprintf(str, MAXLINE, "-->Text:%s-->The number of words is : %d & number of characters is : %d.",msg , wc , cc);
		m = sendto(sockfd, str, strlen(str), 0, pcliaddr, len);
		if (m < 0) {perror("sendto error");}
	} 
}

int main(int argc, char ** argv)
{
	
	int sockfd, b_retVal;
	struct sockaddr_in cliaddr, servaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) perror("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1])); 

	b_retVal = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (b_retVal < 0) { perror("bind: error");exit(0);}

	dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) ;


}
