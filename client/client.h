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
#include <signal.h>
#include <sys/wait.h>

#include "../message.h"
#define STDIN 0


void prompt();
void *get_in_addr(struct sockaddr *sa);
void prompt_userinput(char *buffer, size_t *size, int fdmax);
int C_connection_setup(char *ip, char *port);


bool connected = false;
char SPACE[] = " ";
char QUIT_COMMAND[] = "/quit",
	 LOGIN_COMMAND[] = "/login", 
	 LOGOUT_COMMAND[] = "/logout",
	 JOIN_SESSION_COMMAND[] = "/joinsession",
	 LEAVE_SESSION_COMMAND[] = "/leavesession",
	 CREATE_SESSION_COMMAND[] = "/createsession",
	 LIST_COMMAND[] = "/list";



void prompt(){
	printf("\t>>>");
	fflush(stdout);
}
void prompt_userinput(char *buffer, size_t *size, int fdmax){
	prompt();
	fd_set set;
	FD_ZERO(&set);
	FD_SET(STDIN, &set);
	if(fdmax != STDIN) {
		FD_SET(fdmax, &set);
	}
	if(select(fdmax+1, &set, NULL,NULL,NULL) == -1){
		perror("select");
		exit(4);
	}
	for(int i=fdmax ;i>=0; i--){
		if(FD_ISSET(i, &set)){
			if(i == STDIN){
				fflush(stdin);
				memset(buffer, 0, 2000);	
				getline(&buffer, size, stdin);
				while(strlen(buffer) ==0){
					prompt();
					getline(&buffer, size, stdin);
				}
				if(buffer[strlen(buffer)-1] == '\n'){
					buffer[strlen(buffer)-1] = '\0';
				}
			}
			else{
				memset(buffer, 0, 2000);
				message temp_buffer;
				recv(i, &temp_buffer, sizeof(message), 0);
				printf("\nNew message from %s:\n\t%s\n", temp_buffer.source, temp_buffer.data);
				fflush(stdout);
			}
		}

	}
}

//return socket file descriptor or -1 on connection error
int C_connection_setup(char *ip, char *port){
	struct addrinfo hints, *serveinfo, *p;
	int getaddrCode, sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((getaddrCode = getaddrinfo(ip, port, &hints, &serveinfo)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrCode));
		exit(1);
	}
	
	for(p = serveinfo; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("client: socket");
			continue;
		}
		printf("sockfd is %d\n", sockfd);
		if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}

	if(p == NULL){
		fprintf(stderr, "client: fail to connect\n");
		return -1;
	}
	char addr6[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), addr6, sizeof addr6);
	printf("Connecting to %s\n", addr6);
	freeaddrinfo(serveinfo);
	connected = true;
	return sockfd;
}

void *get_in_addr(struct sockaddr *sa){
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



//does the login request which is set up connection, (2)check response's ack
//returns the socket file descriptor for later send() and recv() operations
int login_request(char *C_ID, char *C_pw, char *ip, char *port){
	int sockfd;
	sockfd = C_connection_setup(ip, port);

	if(sockfd == -1){
		printf("Login failed: connection setup failed\n");
		return -1;
	}
	else {
		message login, buffer;
		login.type = LOGIN;
		strcpy((char *)login.source, C_ID);
		strcpy((char *)login.data, C_pw);
		send(sockfd, &login, sizeof(login), 0);
		int numBytes = recv(sockfd, &buffer, sizeof(buffer), 0);
		if(numBytes != -1){
			printf("%s", buffer.data);
		}
		if(buffer.type == LO_NAK){
			connected = false;
			return -1;
		}
		else if(buffer.type == LO_ACK){
			connected = true;
		}	
		return sockfd;
	}
	
}




//char *parseCommand(char *string, )