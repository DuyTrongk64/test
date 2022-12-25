#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#define BUFF_SIZE 1024

void *client_chat(void *arg) 
{ 
	int clientfd;
	int sendBytes, rcvBytes;
	int check;
	char buff[BUFF_SIZE];

	check = pthread_detach(pthread_self());
	clientfd = (int) arg;
	while(1){
				recv(clientfd, buff, 1024, 0);
				if (rcvBytes < 0){
					perror("\nError: ");
					break;
			}
				sendBytes = send(clientfd, buff, rcvBytes,0);
				if (sendBytes < 0){
					printf("\nError:");
					break;
			}
} 
	return NULL;
}


int main(int argc, char *argv[])
{
  	if (argc != 2){
        printf("\nUsage: %s <Server Port>", argv[0]);
        return 0;
    }
	pthread_t tid;

  	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	int err;

 	 int PORT = atoi(argv[1]);

	socklen_t addr_size;

	char buffer[1024];
 	 char buffer2[1024];
	pid_t childpid;

 	 int inputTime = 1;
 	 char server_mesg[256] = "Server connected successfully!\n";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if(ret < 0){
		printf("Error in binding.\n");
		exit(1);
	}
	printf("Bind to port %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		printf("Listening....\n");
	}else{
		printf("Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		err = pthread_create(&tid, NULL, &client_chat,(void *) newSocket);
		if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread created successfully\n");

	}

	close(sockfd);


	return 0;
}

