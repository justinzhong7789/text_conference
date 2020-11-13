#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>

#include "server.h"
#include "../message.h"

int main(int argc, char** argv){
	if(argc!=1){
		perror("Bad arguments.");
		exit(1);
	}
	connected_client **connected_clients_list = (connected_client **)malloc(sizeof(connected_client **));
	*connected_clients_list = NULL;	
	int sockfd = S_initSetup(argv[0]);
	int fdmax;

	struct sockaddr_storage income_addr;
	socklen_t sockaddrlen;
	//set this socket to nonblocking
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	printf("Server: waiting for incoing connections...\n");
	fd_set master, read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &master);
	fdmax = sockfd;
	while (1)
	{
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL,NULL) == -1{
			perror("select");
			exit(4);
		}
		for(int i=0;i<=fdmax;i++){
			if(FD_ISSET(i, &read_fds)){
				if(i == sockfd){
					//new connection request
					sockaddrlen = sizeof income_addr;
					int new_fd = accept(sockfd, (struct sockaddr *)&income_addr, &sockaddrlen);
					if(new_fd == -1){
						perror("accept");
					}
					else{
						FD_SET(new_fd, &master);
						fdmax = max(new_fd, fdmax);
						printf("New connection established.");
						message buffer;
						int numBytes = recv(new_fd, &buffer, sizeof(message),0);
						if(numBytes != -1){
							connected_client *new_client = create_client(new_fd, income_addr);
							strcpy(new_client->user_id, buffer.user_id);
							registerClient(connected_clients_list, new_client);
							if(buffer.type == LOGIN){
								int check_pw = checkPW(buffer.user_id, buffer.data);
								message response;
								if(check_pw == -2 || check_pw == -1){
									//username is not found in the database, or password in incorrect
									//send NACK
									response.type = LO_NAK;
									strcpy(response.source, SERVER);
								}
								else if(check_pw == 0){
									//pw is correct, send ack
									response.type = LO_ACK;
									strcpy(response.source, SERVER);
								}
								send(new_fd, &response, sizeof(message), 0);
							}
						}
						else{
							perror("recv\n");
						}
					}
					

				}
				else{
					//data from an already connected client

					

				}
			}
		}



	}
		

	free_linked_list(connected_clients_list);
	return 0;
}