//Task 3
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  // structures for networking
#include <pthread.h> 
#define BUFSIZE 128 //declare the array size

void* Receiver(void *arg){//function to receive
	int sd =*(int*)arg;
	int numbytes;
	char buf[BUFSIZE];
	do {
		memset(buf,0,BUFSIZE);//memset clear the memory to the size of the array 
		numbytes = recv(sd,buf,BUFSIZE,0);
		printf("%s", buf);
		if(strstr(buf,"quit") != NULL){//remote control
			printf("..quitting..\n");
			exit(0);//to quit the program
		}
	}while(numbytes>0);

	return 0;
}

void* Sender(void *arg) {//function to send 
	int sd =*(int*)arg;
	char buf[BUFSIZE];
	while(1) {
		memset(buf,0,BUFSIZE);
		fgets(buf,BUFSIZE,stdin);
		unsigned len = strlen(buf);
		buf[len]='\r';
		send(sd,buf,BUFSIZE,0);
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
	server_address.sin_port = htons(9999);
	server_address.sin_addr.s_addr = INADDR_ANY;//accepts any server from kali linux

	bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));//(struct soackaddr) is to silence the warning 

	listen(server_socket,5);//limit up to 5 connection 
	

	int client_socket[256];
	int i=0;
	while (1){

		client_socket[i] = accept(server_socket,NULL,NULL);

		if (pthread_create(&tidReceiver, NULL, &Receiver, &client_socket)) {//create new receiver child socket to handle the connection 
			perror("Fail create Receiver thread");
		}
		if (pthread_create(&tidSender, NULL, &Sender, &client_socket)) {//create a sender child
			perror("Fail create Sender thread");
		}
	}
	//asynchronous connection 
	//advantages of using thread 
	pthread_join(tidReceiver, NULL);
	pthread_join(tidSender, NULL);
	//what for the child to die before exit 

	close(client_socket);
	close(server_socket);
	//close the socket 
	return 0;
}
