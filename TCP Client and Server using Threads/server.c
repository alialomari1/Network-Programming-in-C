// Ali Alomari 134223
#include	<pthread.h>
#include	<sys/socket.h>
#include	<netinet/in.h>	
#include   	<arpa/inet.h>
#include 	<netinet/in.h>
#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<unistd.h>
#include        <time.h>
#include        <sys/wait.h>
#define	LISTENQ	1024
#define	MAXLINE	1000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int counter = 0 ; // clients counter concurrently .

void rvereseArray(char arr[], int size)
{ int start = 0 ;
  int end = size-1 ;
    while (start < end)
    {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}    // end rvereseArray() .

void str_echo(int sockfd) { printf("\n\n");

	int retVal; 
	ssize_t n;
	char buf[MAXLINE];
	int c=0; // request counter .
	
	while ( (n = read(sockfd, buf, MAXLINE)) > 0) {
		buf[n] = 0;
    c++;
		printf("->request#%d: %s", c ,buf);
		rvereseArray(buf , n-1); 
		printf("->response#%d(reverse): %s\n", c ,buf);

		retVal = write(sockfd, buf , n);
		if (retVal < 0) perror("str_echo: write error");}
    
     } //str_echo().

void * doit (void * arg) {
  int c;
int connfd = *((int *) arg);
pthread_detach(pthread_self());
str_echo(connfd);
close(connfd);

c = pthread_mutex_lock(&mutex);
		if(c !=0 ) perror("Error locking a mutex.\n");

    counter--;
     pthread_cond_signal(&cond);

    c = pthread_mutex_unlock(&mutex);
		if(c !=0 ) perror("Error unlocking a mutex.\n");
return(NULL);
} //end doit() .

int main(int argc, char ** argv) {
	
	pid_t pid;
	int listenfd, connfd, l_retVal, b_retVal, retVal;
	socklen_t len;
	struct sockaddr_in cliaddr, servaddr;
int number ;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) perror("main: socket error");
        else printf("socket created successfully!...\n") ;
        
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1])); 
	b_retVal = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (b_retVal < 0) {perror("main: bind error"); return -1;}
       else printf("bind successfully!...\n") ;

	l_retVal = listen(listenfd, LISTENQ);
	if (l_retVal < 0) perror("main: listen error");
	else printf("listen successfully!...\n") ;


int i=0 ; // total clients counter .
int c;
	for ( ; ; ) {   pthread_t tid;
    i++; 
			len = sizeof(cliaddr);
      c = pthread_mutex_lock(&mutex);
		if(c !=0 ) perror("Error locking a mutex.\n");
counter++;
while(counter > 4) pthread_cond_wait(&cond, &mutex);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);
      	if (connfd < 0) perror("main: accept error");
        else printf("conncection accepted !... (Client#%d)" , i);

      pthread_create(&tid, NULL, doit, &connfd);

			c = pthread_mutex_unlock(&mutex);
		if(c !=0 ) perror("Error unlocking a mutex.\n");




	}//end for loop.


} //end main()
