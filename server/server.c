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
								if(check_pw == 0){
									//pw is correct, send ACK, keep connection
									response.type = LO_ACK;
									strcpy(response.source, SERVER);
									strcpy(response.data, "Login success\n");
									send(new_fd, &response, sizeof(message), 0);
									FD_SET(new_fd, &master);
									fdmax = max(new_fd, fdmax);
								}
								else{
									//username is not found in the database
									//send NACK
									response.type = LO_NAK;
									strcpy(response.source, SERVER);
									(check_pw == -2) ? strcpy(response.data, "Username not found\n") :
													  strcpy(response.data, "Login credentials don't match\n");
									send(new_fd, &response, sizeof(message), 0);
									close(new_fd);
								}
							}
						}
						else{
							perror("recv\n");
						}
					}
				}
				else{
					//data from an already connected client
					message buffer;
					int numbytes = recv(i, &message, sizeof(message), 0);
					if(numbytes == -1){
						perror("recv from old connections error\n");
					}
					else{
						if(buffer.type == EXIT){
							connected_client *p;
							for(p = *connected_clients_list; p!= NULL;p = p->next){
								if(strcmp(message.souce, p->user_id) == 0){
									break;
								}
							}
							close(p->fd);
							int deleted_fd = p->fd;
							if(deleteNode(connected_clients_list, p->user_id)){
								//deletion success
								printf("Socket fd: %d successfully closed\n", deleted_fd);
							}
						}
						else if(buffer.type == JOIN){
							
						}
						else if(buffer.type == LEAVE_SESS){
							
						}
						else if(buffer.type == NEW_SESS){
							
						}
						else if(buffer.type == MESSAGE){
							
						}
						else if(buffer.type == QUERY){
							printf("\tAll connected client(s):\n");
							connected_client *p;
							for(p = *connected_clients_list; p!=NULL; p = p->next){
								print(2);
								printf("%s\n", p->user_id);
							}
							
							//then print chat sessions
							

						}
						else {
							perror("Message type not recognized");
						}
					}


				}
			}
		}



	}
		

	free_linked_list(connected_clients_list);
	return 0;
}