//Referred to tutorial code

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
2) connect
3) read/send ...
4) close
*/
int main()
{
 int sock = 0;
 struct sockaddr_in server;
 
 //Create buffers for sending & receiving data
 char textToSend[1024];
 char textToRec[1024];
 
 
 //Specify which protocols to used 
 server.sin_family = AF_INET;     //For IPV4
 server.sin_port = htons(PORT);   //Host to Network Short
 server.sin_addr.s_addr = INADDR_ANY;   //Remote host IP

 //For using on different machines 
 //server.sin_addr.s_addr = inet_addr("192.168.43.112"); 
 
 /*
 Four possible types
 htonl
 htons
 ntohs
 ntohl
 */
 

 
 
 //Create socket FD
 //Returns -ve if error >= 0 otherwise known as socket FD
 sock = socket(AF_INET, SOCK_STREAM, 0);
 /*
 SOCK_STREAM : Data delivery guaranteed, ordered always
               Uses TCP, if cannot deliver then receive error
               Slow
               
 SOCK_DGRAM  : Opposite that of SOCK_STREAM, fast
 */
 //Plug in to the socket(Connect to the server)
 connect(sock, (struct sockaddr * )&server, sizeof(server));
 
 printf("You are now connected to the server\n");
 
 printf("Enter your data w/o space\n");
 scanf("%s", &textToSend);
 
   
 send(sock, textToSend, strlen(textToSend), 0);
 
 printf("Text sent !\nNow Reading \n");
 
// char buffer[1024];

// bzero(buffer,1024);
// memset( buffer, '\0', 1024 ); 
char* buffer=(char*)calloc(1024,1);

int	n=recv(sock,buffer,1024,0);

 while(n>0){
 	if(strcmp(buffer,"No file with the specified file name found on the server!")==0){
 		printf("%s \n", buffer);
 		exit(1);
 	}
 	else{
 	FILE *fp;
	fp=fopen(textToSend,"a");
 	printf("Text Received : %s \n", buffer);
 	fprintf(fp,"%s",buffer);
 	// bzero(buffer,1024);
 	buffer=(char*)calloc(1024,1);
 	// memset( buffer, '\0',1024 ); 
 	n=recv(sock,buffer,1024,0);
 	}
 }

 printf("Now Closing \n");
 
 
 //Close the socket so that port can be reused by other process
 //It is not 
 close(sock);
 
}

//FILE NOT FOUND ON SERVER THEN DONT CREATE IT IN CLIENt DIRECtORY
//AGAIN ASK CLIENT FOR FILENAME IF FILE NOT FOUND