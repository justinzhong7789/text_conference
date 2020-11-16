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

#include "client.h"
int main(int argc, char** argv){

    size_t BUFFER_SIZE = 2000;
    char *buffer = (char *)malloc(BUFFER_SIZE);
    prompt_userinput(buffer, &BUFFER_SIZE);
    int sockfd = -1;
    char *username = NULL;
    while(strcmp(buffer, QUIT_COMMAND) != 0){
        int arg = 0;
        char *command, place_holder[BUFFER_SIZE];
        strcpy(place_holder, buffer);
        command = strtok(place_holder, SPACE);
        arg++;
        printf("aa-%s-aa\n", command);
        if(strcmp(command, LOGIN_COMMAND) == 0){
            char *ID, *PW, *S_IP, *PORT, *argument;
            argument = strtok(NULL, SPACE);
            arg++;
            while (argument != NULL)
            {
                if(arg == 2){
                    ID = argument;
                }
                else if(arg == 3){
                    PW = argument;
                }
                else if(arg == 4){
                    S_IP = argument;
                }
                else if(arg == 5){
                    PORT = argument;
                }
                argument = strtok(NULL, SPACE);
                if(argument != NULL){
                    arg++;
                }
            }
            if(arg > 5){
                printf("You entered too many arguments, try again\n");
            }
            else if(arg<5){
                printf("You entered too few arguments, try again\n");
            }
            else{
                sockfd = login_request(ID, PW, S_IP, PORT);
                if(sockfd != -1){
                    username = (char *)malloc(MAX_NAME);
                    strcpy(username, ID);
                }
            }
        }
        else if(strcmp(command, LOGOUT_COMMAND) == 0){
            if(sockfd == -1){
                printf("You are not connected to any server yet. Login first!\n");
            }
            else{
                message logout_request;
                logout_request.type = EXIT;
                strcpy((char *)logout_request.source, username);
                send(sockfd, &logout_request, sizeof(message), 0);
                close(sockfd);
                free(username);
                username = NULL;
                sockfd = -1;
            }
        }
        else if(strcmp(command, JOIN_SESSION_COMMAND) == 0){
            if(sockfd == -1){
                printf("You are not connected to any server yet. Login first!\n");
            }else{
                //Join Session
                char *session_name, *argument;
                argument = strtok(NULL, SPACE);
                arg++;
                while (argument != NULL)
                {
                    if(arg == 2){
                        session_name = argument;
                    }
                    argument = strtok(NULL, SPACE);
                    if(argument != NULL){
                        arg++;
                    }
                }
                printf("session name in client: %s\n",session_name);

                if(arg > 2){
                    printf("You entered too many arguments, try again\n");
                }
                else if(arg<2){
                    printf("You entered too few arguments, try again\n");
                }
                else{
                    message join_sess_request;
                    join_sess_request.type = JOIN;
                    strcpy((char *)join_sess_request.source, username);
                    strcpy((char *)join_sess_request.data, session_name);
                    send(sockfd, &join_sess_request, sizeof(message), 0);
                }

            }


        }
        else if(strcmp(command, LEAVE_SESSION_COMMAND) == 0){
            if(sockfd == -1){
                printf("You are not connected to any server yet. Login first!\n");
            }else{
                if(arg >= 2){
                    printf("You entered too many arguments, try again\n");
                } else{
                    //Leave session
                    printf("Client %s leaving session.\n", username);
                    message leave_sess_request;
                    leave_sess_request.type = LEAVE_SESS;
                    strcpy((char *)leave_sess_request.source, username);
                    strcpy((char *)leave_sess_request.data, "");//session name is not needed
                    send(sockfd, &leave_sess_request, sizeof(message), 0);
                }
            }
        }
        else if(strcmp(command, CREATE_SESSION_COMMAND)==0){
            //char *session_name = strtok(NULL, SPACE);
            if(sockfd == -1){
                printf("You are not connected to any server yet. Login first!\n");
            }else{
                //create new Session
                char *session_name, *argument;
                argument = strtok(NULL, SPACE);
                arg++;
                while (argument != NULL)
                {
                    if(arg == 2){
                        session_name = argument;
                    }
                    argument = strtok(NULL, SPACE);
                    if(argument != NULL){
                        arg++;
                    }
                }
                printf("session name in client: %s\n",session_name);

                if(arg > 2){
                    printf("You entered too many arguments, try again\n");
                }
                else if(arg<2){
                    printf("You entered too few arguments, try again\n");
                }
                else{
                    message new_sess_request;
                    new_sess_request.type = NEW_SESS;
                    strcpy((char *)new_sess_request.source, username);
                    strcpy((char *)new_sess_request.data, session_name);
                    send(sockfd, &new_sess_request, sizeof(message), 0);
                }
            }
        }
        else if(strcmp(command, LIST_COMMAND) == 0){

        }
        else{
            //send plain text
            if(sockfd == -1){
                printf("You aren't connected to a server yet\n");
            }
            else{
                if(username == NULL){
                    printf("Your username is unclear\n");
                }
                else{
                    
                    message chat;
                    chat.type = MESSAGE;
                    strcpy((char *)chat.source, username);
                    strcpy((char *)chat.data, buffer);
                    send(sockfd, &chat, sizeof(message), 0);
                    
                }
                

            }
        }
        prompt_userinput(buffer, &BUFFER_SIZE);
    }
    free(username);
    free(buffer);

    return 0;
}