#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
using namespace std;

#define PORT 8080
#define MAXLINE 1024

// create the socket
int sock = socket(AF_INET, SOCK_STREAM, 0);

//setup an address
struct sockaddr_in server_address;

	
void ClientEncoder()
{
	FILE* ptr;
    char ch; int i=0;
	char array[MAXLINE];
    ptr = fopen("intext.txt", "r");

	cout << "Reading intext.txt file..\n" << endl;
 
    if (NULL == ptr)
        printf("file can't be opened \n");
 
    while (!feof(ptr)) {
        array[i] = fgetc(ptr);
		i++;
    }

	array[i] = '\0';

	cout << "File contents..\n" << endl;
	cout << array << endl;

	cout << "Sending data to the server.." << endl;
	send(sock, array, strlen(array), 0);
	cout << "\nData sent successfully!\n" << endl;

    fclose(ptr);
}

void ClientDecoder()
{
	char buffer[MAXLINE];
	int n = recv(sock, &buffer, sizeof(buffer), 0);
	buffer[n] = '\0';

	cout << "\nData received from server: " << buffer << endl;

    FILE *filePointer;
    filePointer = fopen("result.txt", "w") ;
	cout << "Writing the recieved data into file..\n" << endl;
     
    if ( filePointer == NULL )
        printf("Failed to open file !");
    else
    {
        if ( strlen (buffer) > 0 )
        {
            fputs(buffer, filePointer) ;
            fputs("\n", filePointer) ;
        }
         
        fclose(filePointer) ;
         
        printf("Data successfully written in file..\n");
	}
}


int main() 
{
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("192.168.100.10");
	server_address.sin_port = htons(PORT);

	connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
	
	cout << "Client Interface" << endl;

	ClientEncoder();
	ClientDecoder();

	close(sock);

	return 0;
}

