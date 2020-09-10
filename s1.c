//Referred to the tutorial code

#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 9001


/*
These functions we will use in order
1) socket
2) bind
3) listen
4) accept
5) read/send ...
6) close
*/

int main()
{
 int socketId, client;
 
 struct sockaddr_in server;
 
 int addLen = sizeof(server);
 
 char textToSend[1024] = {0}, textToRec[1024] = {0};
 
 server.sin_family = AF_INET;
 server.sin_port = htons(PORT);
 server.sin_addr.s_addr = INADDR_ANY;
 
 //socket(DOMAIN, TYPE, PROTOCOL) and returns socket FD
 socketId = socket(AF_INET, SOCK_STREAM, 0);
 
 //Binding port with address
 //Takes socket FD, server structure in generic form (protocol independent) and size of address(server variable in this case)
 
 bind(socketId, (struct sockaddr *)&server, sizeof(server));
 
 //Lets wait for a client(s)
 //listen(socketFD, backlogs)
 //backlogs means maximum number of connections in waiting queue, eg if backlogs = 1, then while serving a connection, if a new connection comes then it will sit in waiting queue if another one comes it will be dropped out,
//If backlogs = 0, then while serving a connection if another connection comes it will be dropped out
 listen(socketId, 0);
 printf("Server ready\n");
 
 //Accepting connection
 client = accept(socketId, (struct sockaddr *)&server, (socklen_t *)&addLen);
 
 printf("Client Connected \n");
 read(client, textToRec, 1024);
 printf("Client : %s\n", textToRec);
 
FILE *f;
f=fopen(textToRec,"r");
if (!f) {
    printf("Could not open file\n");
    close(socketId); //CHECK IF WORKING OR NOT
    exit(1);
}
// char *string = malloc(fsize + 1);
char buffer[1024];
// bzero(buffer,1024);
memset( buffer, '\0', sizeof(buffer) );
// count= fread(&buffer,1024, 1,f);
int count=fread(buffer, 1, 1024, f);
// printf("%s",buffer);
if(count==-1){
	printf("Error in reading from file");
}
// int j=0;
while(count>0){
	// j=j+1;
	// printf("%d",j);
	printf("Text Read: %s",buffer);
	send(client, buffer, strlen(buffer), 0);
	// bzero(buffer,1024);
	memset( buffer, '\0', sizeof(buffer) );
	count=fread(buffer, 1, 1024, f);
}
printf(count);
if(count==-1){
	printf("Error in reading from the file");
}
 
 
 close(socketId);
}