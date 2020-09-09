#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 9002
#define PACKSIZE 1024
int main()
{
 int sock = 0;
 struct sockaddr_in server;
 
 char textToSend[PACKSIZE];
 char textToRec[PACKSIZE];
 
 
 server.sin_family = AF_INET;     //For IPV4
 server.sin_port = htons(PORT);   //Host to Network Short
 server.sin_addr.s_addr = INADDR_ANY;   //Remote host IP

 connect(sock, (struct sockaddr * )&server, sizeof(server));
 
 printf("You are now connected to the server!\n");
 
 printf("Enter the filename\n");
 scanf("%s", &textToSend);
 
   
 send(sock, textToSend, strlen(textToSend), 0);
 printf("Text sent !\nNow Reading \n");

char buffer[PACKSIZE];
FILE *fp;
fp=fopen(textToSend,"w");
// memset( buffer, '\0', sizeof(buffer) ); 
// printf("%d",read(sock,buffer,PACKSIZE));
 int n=4;
 int j=0;
 while(n>0){
 	// j=j+1;
 	printf("%d",j);
 	memset( buffer, '\0', sizeof(buffer) ); 
 	n=recv(sock,buffer,PACKSIZE,0);
 	printf("Text Received : %s", buffer);
 	fprintf(fp,"%s",buffer);
 }
 
 printf("Now Closing \n");
 
 
 //Close the socket so that port can be reused by other process
 //It is not 
 close(sock);
 
}