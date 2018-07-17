#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>//socket library provide endpoints
#include <netinet/in.h>//support new socket
#include <string.h>//support string variable
#include <stdlib.h>
#include <pthread.h>

int main(){
	pthread_t tidsender;
	int sd[256];
	int i=0;
	struct sockaddr_in addr;
	char buf[64]={0};
	memset(buf,0,64);//sets all the bytes to 0 

	addr.sin_family=AF_INET;
	addr.sin_port=htons(9999);//portnumber 9999

	inet_aton("127.0.0.1",&addr.sin_addr);
	sd[i]=socket(PF_INET,SOCK_STREAM,0);//create a server socket

	connect(sd[i],&addr,sizeof(addr));
	while (1){
		fgets(buf,64,stdin);//collect input
		send(sd[i],buf,strlen(buf),0);//send input to server
		if (strstr(buf,"quit")!=NULL){
			exit(0);//exit client server
		}
	}
	if (pthread_create(&tidsender,NULL,NULL,&sd[i])){
		perror("Fail create Sender Thread");
	}
	close(sd[i]);
	return 0;
}
	
