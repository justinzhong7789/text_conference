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
	if(argc!=2){
		perror("Bad arguments.");
		exit(1);
	}
	connected_client **connected_clients_list = (connected_client **)malloc(sizeof(connected_client **));
	*connected_clients_list = NULL;	
	int sockfd = S_initSetup(argv[1]);
	int fdmax;

	//initialize session list
	struct sessionNode* sessionList[MAXNUMSESSIONS];
	int curSessionSize = 0;

	struct sockaddr_storage income_addr;
	socklen_t sockaddrlen;
	//set this socket to nonblocking
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	fd_set master, read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &master);
	fdmax = sockfd;

	while (1)
	{
		printf("Server: waiting for incoming requests...\n");
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL,NULL) == -1){
			perror("select");
			exit(4);
		}
		for(int i=0;i<=fdmax;i++){
			if(FD_ISSET(i, &read_fds)){
				if(i == sockfd){
					//new connection request
					sockaddrlen = sizeof(struct sockaddr_storage);
					int new_fd = accept(sockfd, (struct sockaddr *)&income_addr, &sockaddrlen);
					if(new_fd == -1){
						perror("accept");
					}
					else{
						printf("New connection established.");
						message buffer;
						int numBytes = recv(new_fd, &buffer, sizeof buffer,0);
						if(numBytes != -1){
							
							if(buffer.type == LOGIN){
								int check_pw = checkPW((char *)buffer.source, (char *)buffer.data);
								message response;
								if(check_pw == 0){
									//pw is correct, send ACK, keep connection
									response.type = LO_ACK;
									strcpy((char *)response.source, SERVER);
									strcpy((char *)response.data, "Login credentials match. Login successful\n");
									send(new_fd, &response, sizeof(message), 0);
									FD_SET(new_fd, &master);
									fdmax = (new_fd>fdmax)? new_fd: fdmax;	
									connected_client *new_client = create_client(new_fd, income_addr);
									strcpy(new_client->user_id, (char*)buffer.source);
									registerClient(connected_clients_list, new_client);							}
								else{
									//username is not found in the database
									//send NACK
									response.type = LO_NAK;
									strcpy((char *)response.source, SERVER);
									(check_pw == -2) ? strcpy((char *)response.data, "Username not found\n") :
													   strcpy((char *)response.data, "Login credentials don't  match\n");
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
					int numbytes = recv(i, &buffer, sizeof(message), 0);
					if(numbytes == -1){
						perror("recv from old connections error\n");
					}
					else{
						//EXIT = LOGOUT
						if(buffer.type == EXIT){
							connected_client *p;
							for(p = *connected_clients_list; p!= NULL;p = p->next){
								if(strcmp((char *)buffer.source, p->user_id) == 0){
									break;
								}
							}
							close(p->fd);
							int deleted_fd = p->fd;
							if(deleteNode(connected_clients_list, p->user_id)){
								//deletion success
								printf("Socket fd: %d successfully closed\n", deleted_fd);
							}
							fd_set fds;
							FD_ZERO(&fds);
							int old_fdmax = fdmax, new_fdmax;
							for(int i=0;i<= old_fdmax; i++ ){
								if(FD_ISSET(i, &master) && i!=deleted_fd){
									FD_SET(i, &fds);
									new_fdmax = (i>fdmax)?i:fdmax;
								}
							}
							fdmax = new_fdmax;
							master = fds;
						}
						else if(buffer.type == JOIN){
							printf("Server handling join request.\n");
							char* sessionName = (char*)buffer.data;
							char* clientID = (char*)buffer.source; //username
							printf("Session name %s, client ID %s.\n",sessionName, clientID);

							//Look for the session of the client
							if (findSessionOfClient(sessionList, &curSessionSize, clientID) !=-1){
								printf("Client already joined a session\n");
								continue;
							} 
							//Look for session in existing list
							int sessionIdx = findSessionByName (sessionList, &curSessionSize, sessionName);
							if (sessionIdx==-1){
								printf("Session does not exist.\n");
								continue;
							}
							//Insert sockID to the session
							sessionList[sessionIdx]->sockfds[sessionList[sessionIdx]->curNumClients]=sockfd;
							//Insert client ID to the session
							strcpy(sessionList[sessionIdx]->clientIDs[sessionList[sessionIdx]->curNumClients], clientID);
							sessionList[sessionIdx]->curNumClients =sessionList[sessionIdx]->curNumClients+1;
							printf("Client %s joined session %s.\n", clientID, sessionName);

							printf("-----------Session List ---------\n");
							printAllSessions(sessionList, &curSessionSize);
							printf("---------------------------------\n");

						}
						else if(buffer.type == NEW_SESS){
							//Need to modify message in client
							char* sessionName = (char*)buffer.data;
							char* clientID = (char*)buffer.source;
							//Validate Client ID
							if (strcmp(clientID, "")==0){
								printf("Client ID is empty.\n");
								continue;
							}
							//Look for client in already connected sessions
							if (findSessionOfClient(sessionList, &curSessionSize, clientID)!=-1){
								printf("Client already joined a session.\n");
								continue;
							}
							//Look for the session in the existing list
							if (findSessionByName(sessionList, &curSessionSize, sessionName)!=-1){
								printf("Session exists.\n");
								continue;
							}
							//Session has not been created
							struct sessionNode* newSession = (struct sessionNode*)malloc(sizeof(struct sessionNode));
							newSession->sessionName = (char*)malloc(MAXSIZESESSIONNAME*sizeof(char));
							strcpy(newSession->sessionName, sessionName);
							newSession->sockfds  = (int*)malloc(MAXNUMCLIENTS*sizeof(int));
							newSession->sockfds[0]=sockfd;
							newSession->clientIDs = (char**)malloc(MAXNUMCLIENTS*sizeof(char*));
							for(int n=0; n<MAXNUMCLIENTS; n++){
								newSession->clientIDs[n]=malloc(MAXSIZECLIENTID*sizeof(char));
							}
							strcpy(newSession->clientIDs[0], clientID);
							newSession->curNumClients=1;
							//Insert to the session list
							if (insertSession(sessionList, &curSessionSize, newSession)==-1){
								printf("Can not insert.\n");
								continue;
							}
							printf("Client %s created and joined session %s.\n", clientID, sessionName);
							printf("-----------Session List ---------\n");
							printAllSessions(sessionList, &curSessionSize);
							printf("---------------------------------\n");
							
						}
						else if(buffer.type == LEAVE_SESS || buffer.type == QUIT){
							char* clientID = (char*)buffer.source;
							//Look for client in already connected sessions
							int sessionIdx = findSessionOfClient(sessionList, &curSessionSize, clientID);
							if (sessionIdx==-1){
								printf("Client did not connect to any session.\n");
								continue;
							}
							printf("Client %s leaving the session.\n", clientID);
							int clientIdx = findIdxOfClient(sessionList, clientID, sessionIdx);//helper function in session.c
							removeClientID(sessionList, clientIdx, sessionIdx);

							//Free session node if all clients have left
							if (sessionList[sessionIdx]->curNumClients==0){
								deleteSession(sessionList, &curSessionSize, sessionIdx);
							}
							printf("-----------Session List ---------\n");
							printAllSessions(sessionList, &curSessionSize);
							printf("---------------------------------\n");
							
						}
						else if(buffer.type == MESSAGE){
							//check if client is in any session
							char* clientID = (char*)buffer.source;
							char* clientMsg = (char*)buffer.data;
							message context;
							strcpy((char *)context.source, clientID);
							strcpy((char *)context.data, clientMsg);
							context.size = strlen(clientMsg);
							int sessionIdx = findSessionOfClient(sessionList, &curSessionSize, clientID); //Find the index of session client is in
							if (sessionIdx==-1){
								printf("Client did not connect to any session.\n");
								continue;
							}
							printf("%s said: %s\n", buffer.source, buffer.data);							
							//Iterate through all client IDs in this session
							for (int n=0; n<sessionList[sessionIdx]->curNumClients; n++){
								//char* curClientID = sessionList[sessionIdx]->clientIDs[n];
								//send the message to this client
								//sockfd invalid, broken pipe
								//send(sessionList[sessionIdx]->sockfds[n], &context, sizeof(message),0);
							}
						}

						else if(buffer.type == QUERY){
							//may have risk of overflow
							int loc = 0;
							message list_response;
							strcpy((char *)list_response.source, SERVER);
							list_response.type = QUERY;
							connected_client *p;
							char prompt1[] = "All connected clients:\n",
								 prompt2[] = "All sessions available:\n";
							strcpy((char *)list_response.data + loc, prompt1);
							loc += strlen(prompt1);
							for(p=*connected_clients_list; p!=NULL; p=p->next){
								strcpy((char *)list_response.data+loc, p->user_id);
								loc+=strlen(p->user_id);
								list_response.data[loc] = '\n';
								loc++; 
							}
							strcpy((char *)list_response.data+loc, prompt2);
							loc+= strlen(prompt2);
							//output in format:
							//session_name:
							//		client1
							//		client2...
							for(int i=0;i<curSessionSize;i++){
								strcpy((char *)list_response.data+loc, sessionList[i]->sessionName);
								loc+=strlen(sessionList[i]->sessionName);
								list_response.data[loc] = ':';
								loc++;
								list_response.data[loc] = '\n';
								loc++;
								for(int j=0;j<sessionList[i]->curNumClients; j++){
									list_response.data[loc] = '\t';
									loc++;
									strcpy((char *)list_response.data+loc, sessionList[i]->clientIDs[j]);
									loc += strlen(sessionList[i]->clientIDs[j]);
									list_response.data[loc] = '\n';
									loc++;
								}
							}
							list_response.data[loc] = '\0';
							send(i, &list_response, sizeof(message), 0);
							
						}
						else {
							perror("Message type not recognized");
						}
					}


				}
			}
		}



	}
		
	deleteAllSessions(sessionList);
	free_linked_list(connected_clients_list);
	return 0;
}
