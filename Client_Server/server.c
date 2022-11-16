// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include<pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 3359
#define MAXLINE 1024

char buffer[MAXLINE] = "";
int SUM = 0;

pthread_t ptid;

int sockfd;
struct sockaddr_in servaddr, cliaddr;
int len = sizeof(cliaddr);

void* ThreadFunction(void* arg)
{
    // detach the current thread
    // from the calling thread
    pthread_detach(pthread_self());

	int i = *(int*)arg;
	buffer[i] = toupper(buffer[i]);
    
    // exit the current thread
    pthread_exit(NULL);
}

void ServerEncoder(int n)
{
	printf("Sending Decoded Data to corresponding client..\n");
	sendto(sockfd, (const char *)buffer, strlen(buffer)-1, 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
	printf("Data sent successfully !");
}

int ServerDecoder()
{
	int n = recvfrom(sockfd, (char *)buffer, MAXLINE,0, ( struct sockaddr *) &cliaddr, &len);
	printf("Data recieved from the client: %s\n", buffer);
	buffer[n] = '\0'; int i=0;

	printf("\nDecoding the data recieved from the client..\n");

	while(i < n-1)
	{
		int* a = malloc(sizeof(int));
		*a = i;

		if( buffer[i] == 'a')
			pthread_create(&ptid, NULL, &ThreadFunction, a);
			// buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'e')
			pthread_create(&ptid, NULL, &ThreadFunction, a);
			// buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'i')
			pthread_create(&ptid, NULL, &ThreadFunction, a);
			// buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'o')
			pthread_create(&ptid, NULL, &ThreadFunction, a);
			// buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'u')
			pthread_create(&ptid, NULL, &ThreadFunction, a);
			// buffer[i] = toupper(buffer[i]);
		else if(buffer[i] >= 48 && buffer[i] <= 57)
			SUM += (buffer[i]-48);

		pthread_join(ptid, NULL);
		//printf("%s", buffer[i]);
		//printf("%d", i);
		i++;
	}

	printf("\nSum of digits: %d\n", SUM);

	return n;
}


// Driver code
int main() 
{
	printf("Server Interface\n");

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	//memset(&servaddr, 0, sizeof(servaddr));
	//memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	while(1)
	{
		int n = 0;
		int fork_id = fork();

		if(fork_id == 0) //child process 1
		{	
			n = ServerDecoder();		
			ServerEncoder(n);
		}		
		else if(fork_id == 1)//child process 2
		{
			// wait(NULL);
			// ServerEncoder(n);
		}
	}
	
	close(sockfd);		
	return 0;
}
