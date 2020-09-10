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
 if((socketId = socket(AF_INET, SOCK_STREAM, 0))<0){
 	perror("Error in creating server socket!");
 	exit(1);
 }
 
 //Binding port with address
 //Takes socket FD, server structure in generic form (protocol independent) and size of address(server variable in this case)
 
 if((bind(socketId, (struct sockaddr *)&server, sizeof(server)))<0){
 	perror("Error in binding server socket to localhost!");
 	exit(1);
 }
 
 //Lets wait for a client(s)
 //listen(socketFD, backlogs)
 //backlogs means maximum number of connections in waiting queue, eg if backlogs = 1, then while serving a connection, if a new connection comes then it will sit in waiting queue if another one comes it will be dropped out,
//If backlogs = 0, then while serving a connection if another connection comes it will be dropped out
 if(listen(socketId, 0)<0){
 	perror("Error in listening@");
 	exit(1);}
 printf("Server ready\n");
 
 //Accepting connection
 if((client = accept(socketId, (struct sockaddr *)&server, (socklen_t *)&addLen))<0){
 	perror("Error in accepting a connection!");
 	exit(1);
 }
 printf("Client Connected \n");

 if(read(client, textToRec, 1024)<0){
 	perror("Error in reading from the client!");
 	exit(1);
 }
 printf("Client : %s\n", textToRec);
 
FILE *f;
f=fopen(textToRec,"r");
// char buffer[1024];
char* buffer=(char*)calloc(1024,1);
// bzero(buffer,1024);
// memset( buffer, '\0', 1024 );
if (!f) {
    printf("Requested file not found!\n");
    strcpy(buffer,"No file with the specified file name found on the server!");
    send(client,buffer,57,0);
    exit(1);
}

int count=fread(buffer, 1, 1024, f);

if(count==-1){
	printf("Error in reading from file");
}
while(count>0){
	printf("Text Read: %s",buffer);
	send(client, buffer, 1024, 0);
	// bzero(buffer,1024);
	buffer=(char*)calloc(1024,1);
	// memset( buffer, '\0', 1024 );
	count=fread(buffer, 1, 1024, f);
}
printf(count);
if(count==-1){
	printf("Error in reading from the file");
}
close(socketId);
}