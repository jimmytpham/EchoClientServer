/* Jimmy Pham
*  T00629354
*  Assignment 2
*/

//TCP Echo server program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	BUFLEN	512											// Maximum length of buffer
#define PORT	8899										// Fixed server port number

int main (void)
{
	struct sockaddr_in server_address;						// Data structure for server address
	struct sockaddr_in client_address;						// Data structure for client address
	int client_address_len = 0;
		
	// Populate socket address for the server
	memset (&server_address, 0, sizeof (server_address));	// Initialize server address data structure
	server_address.sin_family = AF_INET; 					// Populate family field - IPV4 protocol
	server_address.sin_port = PORT;							// Set port number
	server_address.sin_addr.s_addr = INADDR_ANY;			// Set IP address to IPv4 value for loacalhost
	 	
	// Create a TCP socket; returns -1 on failure
	int listen_sock;
	if ((listen_sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error: Listen socket failed!\n");
		exit (1);
	}

	// Bind the socket to the server address; returns -1 on failure
	if ((bind(listen_sock, (struct sockaddr *)&server_address, sizeof (server_address))) == -1) {
		printf("Error: binding failed!\n");
		exit (1);
	}
	
	// Listen for connections
	if (listen(listen_sock, 5) == -1) { 	// set backlog to 5
        perror("Error: listen failed!\n");
        close(listen_sock); 				// Close the socket before exiting
        exit(1);
    }
	else
		printf("Server is running and waiting for connections...\n");

	// Accept connections from client
	int client_sock;
    if ((client_sock = accept(listen_sock, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len)) == -1) {
        perror("Error: accept failed!\n");
        close(listen_sock); 				// Close the listening socket before exiting
        exit(1);
    }
	else
    	printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));


	// Receive and format the data
	char buffer[BUFLEN];
    ssize_t bytes_received;
    while ((bytes_received = recv(client_sock, buffer, BUFLEN, 0)) > 0) {
        send(client_sock, buffer, bytes_received, 0); 			// Echo back the received data
    }
    
    if (bytes_received == 0) {
        printf("Client disconnected.\n");
    } else if (bytes_received == -1) {
        perror("Error: recv failed!\n");
    }


	
	close (client_sock);			
	close (listen_sock);									// Close descriptor referencing server socket
	
} // End main
