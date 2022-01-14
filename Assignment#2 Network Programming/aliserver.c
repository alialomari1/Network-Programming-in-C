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
#define	MAXLINE	8192

int validop(char *op ); // to check the validity of operation .
int function(char *op, int *n); //to do the calculation .
void split(char line[MAXLINE] ,char *op, int *n , int *spaces);// to split the line .

void str_echo(int sockfd , FILE *fp) { printf("\n\n\n");
    char operation[10];
    int numbers[6];
	int retVal; 
	ssize_t n;
	char buf[MAXLINE];
	int c=0;
	

	while ( (n = read(sockfd, buf, MAXLINE)) > 0) {
		buf[n] = 0;
		c++;//operations counter .
		int spaces;
    split(buf , operation , numbers , &spaces);
   
    fprintf(fp , "-Operation#%d :\t%s ", c , buf);
    
    int valid =validop(operation );
    if((valid == 1) && (spaces ==6) ) fprintf(fp , "(op#%d Successfull)\n" , c);
    else fprintf(fp , "(op#%d Unsuccessfull)\n" , c);
    
    int result = function(operation , numbers);
     char sresult[10] ;
     sprintf(sresult, "%d", result);
     
		printf("Received text is: %s\n", buf);
		
		if((valid == 1) && (spaces ==6)){// if the operation is valid .
		retVal = write(sockfd, sresult , n);
		if (retVal < 0) perror("str_echo: write error");}
		
		else{// if the operation is not valid .
		retVal = write(sockfd, "Unsupported Operation" , sizeof("Unsupported Operation"));
		if (retVal < 0) perror("str_echo: write error");}
		
	} 
	if (n < 0) perror("str_echo: read error");
}//str_echo()

int main(int argc, char ** argv) {

FILE *fp = fopen("servfile.txt" , "w+");
	
	pid_t pid;
	int listenfd, connfd, l_retVal, b_retVal, retVal;
	socklen_t len;
	struct sockaddr_in cliaddr, servaddr;
int number ;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) perror("main: socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1])); 
	b_retVal = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (b_retVal < 0) perror("main: bind error");

	l_retVal = listen(listenfd, LISTENQ);
	if (l_retVal < 0) perror("main: listen error");
int i=0 ; // clients counter .
int j=0; //clients concurrently counter .
	for ( ; ; ) {   i++; 
			len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);
			j++;
			if (connfd < 0) perror("main: accept error");

			if( (pid = fork()) == 0 ) {
fprintf(fp , "\n\nClient #%d : \n" , i);

                       time_t rawtime;
                       time (&rawtime);
    fprintf(fp ,"Date and Time is : %s" , ctime(&rawtime)); // Date and Time .


			char str[INET_ADDRSTRLEN];
inet_ntop( AF_INET, &(cliaddr.sin_addr), str, INET_ADDRSTRLEN );
fprintf(fp ,"The client’s IP is : %s\n\n" , str);// client's IP

			
				retVal = close(listenfd); 	
				if (retVal < 0) perror("main: child close error");
				str_echo(connfd , fp); 
			
				
				exit(0);
				
			}
else{
if(j>3)     if ((pid = wait(&number)) == -1)
                   perror("wait() error");

else j--;}
			retVal = close(connfd);
			
		if (retVal < 0) perror("main: parent close error");
	
	}//end for loop.
	fclose(fp);

} //end main()





void split(char line[MAXLINE] ,char *op, int *n , int *spaces){

int s=0;
int space[6];
int len = strlen(line);
char temp[10];
        for (int i = 0; i < len; i++)
        {
            if (line[i] == ' ')
            {
                space[s] = i;
                s++;
            }
        }
        *spaces =s;

for (int i = 0; i < space[0]; i++)
           op[i] = line[i];
           op[space[0]] = '\0';
//printf("%s\n" , op);

for (int i = space[0]+1; i < space[1]; i++)
           temp[i - (space[0]+1)] = line[i];
           temp[space[1] - space[0] - 1] = '\0';
           n[0] = atoi(temp);
//printf("%d\n" , n[1]);

for (int i = space[1]+1; i < space[2]; i++)
           temp[i - (space[1]+1)] = line[i];
           temp[space[2] - space[1] - 1] = '\0';
           n[1] = atoi(temp);
//printf("%d\n" , n[2]);

for (int i = space[2]+1; i < space[3]; i++)
           temp[i - (space[2]+1)] = line[i];
           temp[space[3] - space[2] - 1] = '\0';
           n[2] = atoi(temp);
//printf("%d\n" , n[3]);

for (int i = space[3]+1; i < space[4]; i++)
           temp[i - (space[3]+1)] = line[i];
           temp[space[4] - space[3] - 1] = '\0';
           n[3] = atoi(temp);
//printf("%d\n" , n[4]);

for (int i = space[4]+1; i < space[5]; i++)
           temp[i - (space[4]+1)] = line[i];
           temp[space[5] - space[4] - 1] = '\0';
           n[4] = atoi(temp);
//printf("%d\n" , n[5]);

for (int i = space[5]+1; i < len; i++)
           temp[i - (space[5]+1)] = line[i];
           temp[len - space[4] - 1] = '\0';
           n[5] = atoi(temp);
//printf("%d\n" , n[6]);


}//end split()



int function(char *op ,int *n){
int s=0;
//printf("%s\n" ,op );
  if (strcmp(op, "EVN") == 0){
for (int i=0 ; i<6 ; i++) if(n[i]%2==0) s+=n[i];}

else if (strcmp(op, "ODD") == 0){
for (int i=0 ; i<6 ; i++) if(n[i]%2==1) s+=n[i];}

return s;

} //end function()



int validop(char *op ){
if (strcmp(op, "EVN") == 0) return 1;
else if (strcmp(op, "ODD") == 0) return 1;
else return 0;
} // validop()


