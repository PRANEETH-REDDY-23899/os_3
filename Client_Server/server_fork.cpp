#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctime>
#include<fstream>
#include <sys/wait.h>
#include <netinet/in.h>
#include<iostream>
using namespace std;

#define PORT 8080
#define MAXLINE 1024
char buffer[MAXLINE] = "";

// create the server socket
int server_socket = socket(AF_INET, SOCK_STREAM, 0);
int client_socket;

// define the server address
struct sockaddr_in server_address;

 
void ServerEncoder()
{
	cout << "Sending Decoded Data to corresponding client..\n" << endl;
	send(client_socket, buffer, strlen(buffer)-1, 0);
	cout << "Data sent successfully !" << endl;

}

char* ServerDecoder()
{
	int n = recv(client_socket, &buffer, MAXLINE, 0);
	cout << "Data recieved from the client: " << buffer << endl;
	buffer[n] = '\0'; int i=0;

	cout << "\nDecoding the data recieved from the client..\n" << endl;

	while(i < n-1)
	{
		if( buffer[i] == 'a')
			buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'e')
			buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'i')
			buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'o')
			buffer[i] = toupper(buffer[i]);
		else if( buffer[i] == 'u')
			buffer[i] = toupper(buffer[i]);

		cout << buffer[i];
		cout << i << endl;
		i++;
	}
}


int main() 
{
	cout << "Server Interface\n" << endl;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to our specified IP and port
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	listen(server_socket, 5);

	while(1)
	{
		client_socket = accept(server_socket, NULL, NULL);
		
		int fork_id = fork();

		if(fork_id == 0) //child process 1
		{	
			ServerDecoder();		
			ServerEncoder();	

			close(client_socket);	
		}		
		else if(fork_id == 1)//child process 2
		{
			//close(client_socket);
		}
	}
	
	// close the socket
	close(server_socket);
	
	return 0;
}
