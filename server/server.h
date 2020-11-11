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
#define DATABASE "database_txt"
#define CHAR_SIZE 1
#define BACK_LOG 10

void writeToDatabase(char *username, char *password);
int checkPW(char *username, char *inputpw);
void sigchild_handler(int s);


//write a new line to databasefile (create this file if it does not exist), in the format:
//<username> <password>
void writeToDatabase(char *username, char *password){
    FILE *dbfile = fopen(DATABASE, "a");
    if(dbfile == NULL){
        printf("dbfile is NULL");
    }
    fwrite(username, CHAR_SIZE, strlen(username), dbfile);
    fputc(' ', dbfile);
    fwrite(password, CHAR_SIZE, strlen(password), dbfile);
    fputc('\n', dbfile);
    fclose(dbfile);
}


// if username entered if not found, return -2,
// if password is incorrect, return -1
// if password is correnct,  return 0
int checkPW(char *username, char *inputpw){

    FILE *dbfile = fopen(DATABASE, "r");
    rewind(dbfile);
    if(dbfile == NULL){
        printf("dbfile is NULL\n");
    }
    size_t buf_size = 50;
    char *buffer = (char *)malloc(buf_size);
    while ((getline(&buffer, &buf_size, dbfile)!=-1)){

        if(buffer[strlen(buffer)-1] == '\n'){
            buffer[strlen(buffer)-1] = '\0';
        }
        char delim = ' ';
        char *token = strtok(buffer, &delim);
        if(strcmp(token, username) ==0){
            token = strtok(NULL, &delim);
            if(strcmp(token, inputpw)==0){
                printf("Login credentials match!\n");
                free(buffer);
                fclose(dbfile);
                return 0;
            }
            else{
                printf("Login credentials do not match!\n");
                fclose(dbfile);
                free(buffer);
                return -1;
            }
        }
        
    }
    printf("Your login username is not found in the database!\n");
    fclose(dbfile);
    free(buffer);
    return -2;

}


//does the initialization setup of the server
//takes care of: socket(), setsockopt(), bind(), listen(), and sigchld_handler()
int initSetup(char *portNum){
	struct addrinfo hints, *serveinfo, *p;
	struct sigaction sa;
	int rv, sockfd, yes=1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((rv = getaddrinfo(NULL, portNum, &hints, &serveinfo)) != 0 ){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}
	for(p = serveinfo; p!= NULL; p=p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("server: socket");
			continue;
		}
		if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes)) == -1){
			perror("setsockopt");
			exit(1);
		}

		if((bind(sockfd, p->ai_addr, p->ai_addrlen)) == -1){
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	freeaddrinfo(serveinfo);
	if(p == NULL){
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}
	if(listen(sockfd, BACK_LOG) == -1){
        perror("listen");
        exit(1);
    }
    sa.sa_handler = sigchild_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }
    return sockfd;
}

void sigchild_handler(int s){
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) >0);
    errno = saved_errno;
}