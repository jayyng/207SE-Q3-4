//Task 4
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  // structures for networking
#include <pthread.h> 
#define BUFSIZE 128

void* Receiver(void *arg){
	int sd =*(int*)arg;
	int numbytes;
	char buf[BUFSIZE];
	do {
		memset(buf,0,BUFSIZE);//set all the bytes to 0 
		numbytes = recv(sd,buf,BUFSIZE,0);//receive bytes from Sender
		printf("%s", buf);
		if(strstr(buf,"quit") != NULL){
			printf("..quitting..\n");
			printf("Thanks for using me\n");
			exit(0);//exit the server
		}
		if(strstr(buf,"showcodes") != NULL){
			system("cat systemprogramming.c");
			//show the codes for this file
		
			
		}
		if(strstr(buf,"showlist") != NULL){
			system("ls");
			//show the list of files in the directory
			
		}
		if(strstr(buf,"shutdown") != NULL){
			system("sudo poweroff");
			//shutdown kali linux
			
		}
		if(strstr(buf,"start vsftpd") != NULL){
			system("service vsftpd start");
			//start port 21
			
		}
		if(strstr(buf,"check netstat") != NULL){
			system("netstat -tan");
			//check for network statistics
		}
		if (strstr(buf,"stop vsftpd")!=NULL){
			system("service vsftpd stop");
			//stop port 21
		}
		
		int cmd = atoi(buf);
		switch (cmd) {
			case 0:
				printf("case 0\n");
				break;
			case 1:
				printf("case 1\n");
				break;
			default:
				printf("default case\n");
		}
	}while(numbytes>0);

	return 0;
}

void* Sender(void *arg) {
	int sd =*(int*)arg;
	char buf[BUFSIZE];
	while(1) {
		memset(buf,0,BUFSIZE);
		fgets(buf,BUFSIZE,stdin);//collect input 
		unsigned len = strlen(buf);
		buf[len]='\r';
		send(sd,buf,BUFSIZE,0);//send bytes to Receiver
	}
}


int main(){
	pthread_t tidReceiver, tidSender;
	
	char server_message[256] = "You have reached server\n";
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0); // 0 default TCP

	int one = 1;
	// SOL_SOCKET = socket options, one = 1 means enable 
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one));

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9999);//portnumber is 9999
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

	listen(server_socket,5);
	

	int client_socket;
	client_socket = accept(server_socket,NULL,NULL);

	if (pthread_create(&tidReceiver, NULL, &Receiver, &client_socket)) {//error handling 
		perror("Fail create Receiver thread");//error message 
	}
	if (pthread_create(&tidSender, NULL, &Sender, &client_socket)) {//error handling 
		perror("Fail create Sender thread");//error message 
	}

	pthread_join(tidReceiver, NULL);//wait for thread termination 
	pthread_join(tidSender, NULL);


	close(client_socket);
	close(server_socket);
	return 0;
}
