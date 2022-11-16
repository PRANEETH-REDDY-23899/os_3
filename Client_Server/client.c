// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 3359
#define MAXLINE 1024

int sockfd;
struct sockaddr_in servaddr;

void ClientEncoder()
{
	FILE* ptr;
    char ch; int i=0;
	char array[MAXLINE];
    ptr = fopen("intext.txt", "r");

	printf("Reading intext.txt file..\n");
 
    if (NULL == ptr)
        printf("file can't be opened \n");
 
    while (!feof(ptr)) {
        array[i] = fgetc(ptr);
		i++;
    }

	array[i] = '\0';

	printf("File contents..\n");
	printf("%s", array);

	printf("Sending data to the server..\n");
	sendto(sockfd, (const char *)array, strlen(array), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	printf("\nData sent successfully!\n");

    fclose(ptr);
}

void ClientDecoder()
{
	char buffer[MAXLINE]; int len = sizeof(servaddr);
	int n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len);
	buffer[n] = '\0';

	int SUM = 0;

	printf("\nData received from server: \n%s", buffer);

		// int i=0;

		// while(i> n-1)
		// {
		// 	if(buffer[i] >= 48 && buffer[i] <= 57)
		// 		SUM += buffer[i]-48;

		// 	i++;
		// }


    FILE *filePointer;
    filePointer = fopen("result.txt", "w") ;
	printf("\n\nWriting the recieved data into file..\n");
     
    if ( filePointer == NULL )
        printf("Failed to open file !");
    else
    {
        if ( strlen (buffer) > 0 )
        {
            fputs(buffer, filePointer);
            fputs("\n", filePointer);
			// fputs(SUM, filePointer);
        }
         
        fclose(filePointer) ;
         
        printf("Data successfully written in file..\n");
	}
}


// Driver code
int main() {
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	//memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("192.168.100.10");

	printf("Client Interface");

	int fork_id = fork();

	if(fork_id == 0) //child process 1
	{		
		ClientEncoder();
		ClientDecoder();
	}
	else if(fork_id == 1) // child process 2
	{
		//wait(NULL);
		//ClientDecoder();
	}


	close(sockfd);
	return 0;
}