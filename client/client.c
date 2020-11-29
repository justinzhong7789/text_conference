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

    size_t BUFFER_SIZE = 2000, buffer_size_temp;
    char *buffer = (char *)malloc(BUFFER_SIZE);
    fd_set master;
    FD_ZERO(&master);
    FD_SET(STDIN, &master);
    int sockfd = -1, fdmax = STDIN;
    buffer_size_temp = BUFFER_SIZE;
    char *username = NULL;
    int sessionNum = 0;
    prompt_userinput(buffer, &buffer_size_temp, fdmax, username );
    
    
    while(strcmp(buffer, QUIT_COMMAND) != 0){
        int arg = 0;
        char *command, place_holder[BUFFER_SIZE];
        strcpy(place_holder, buffer);
        command = strtok(place_holder, SPACE);
        if(command == NULL){
            continue;
        }
        arg++;
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
                    printf("%d\n", sockfd);
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
                if(arg > 2){
                    printf("You entered too many arguments, try again\n");
                }
                else if(arg<2){
                    printf("You entered too few arguments, try again\n");
                }
                else{
                    printf("Sending message to join session %s\n",session_name);
                    message join_sess_request, response;
                    join_sess_request.type = JOIN;
                    strcpy((char *)join_sess_request.source, username);
                    strcpy((char *)join_sess_request.data, session_name);
                    send(sockfd, &join_sess_request, sizeof(message), 0);
                    //if(in_session == NULL){ 
                    //sessionNum++;
                    //}
                    //strcpy(in_sessions[sessionNum], session_name);

                    recv(sockfd, &response, sizeof(message), 0);
                    if (strcmp((char*)response.data, "Client joined the session.\n")==0){
                        sessionNum++;
                        printf("Joined the session. You are now in %d session(s).\n", sessionNum);
                    }else{
                        printf("%s", response.data);
                    }

                }

            }
        }
        else if(strcmp(command, LEAVE_SESSION_COMMAND) == 0){
            if(sockfd == -1){
                printf("You are not connected to any server yet. Login first!\n");
            }else{
                //leave one session
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
                if(arg > 2){
                    printf("You entered too many arguments, try again\n");
                }
                else if(arg<2){
                    printf("You entered too few arguments, try again\n");
                }
                else{
                    printf("Sending message to leave session %s\n",session_name);
                    message leave_sess_request, response;
                    leave_sess_request.type = LEAVE_SESS;
                    strcpy((char *)leave_sess_request.source, username);
                    strcpy((char *)leave_sess_request.data, session_name);
                    send(sockfd, &leave_sess_request, sizeof(message), 0);
                    
                    //Remove the session
                    // for (int i = 0; i<MAXNUMSESSIONS; i++){
                    //     if (strcmp(in_sessions[i], session_name)==0){
                    //         for (int j = i; j<MAXNUMSESSIONS-1; j++){
                    //             strcpy(in_sessions[j], in_sessions[j+1]);
                    //         }
                    //         sessionNum--;
                    //         continue;
                    //     }
                    // }
                    // if(in_session == NULL){ 
                    //     in_session = (char *) malloc(MAX_NAME);
                    // }
                    //strcpy(in_session, session_name);


                    recv(sockfd, &response, sizeof(message), 0);
                    if (strcmp((char*)response.data, "You have left the session.\n")==0){
                        sessionNum--;
                    }
                    printf("%s", response.data);
                
                }

            }
        }
        else if(strcmp(command, LEAVE_ALL_SESSIONS_COMMAND) == 0){
            if(sockfd == -1){
                printf("You are not connected to any server yet. Login first!\n");
            }else{
                if(arg >= 2){
                    printf("You entered too many arguments, try again\n");
                } else{
                    //Leave session
                    printf("Client %s leaving all sessions.\n", username);
                    message leave_all_sess_request, response;
                    leave_all_sess_request.type = LEAVE_ALL_SESS;
                    strcpy((char *)leave_all_sess_request.source, username);
                    strcpy((char *)leave_all_sess_request.data, "");//session name is not needed
                    send(sockfd, &leave_all_sess_request, sizeof(message), 0);

                    // for (int i = 0; i<MAXNUMSESSIONS; i++){
                    //     free(in_sessions[i]);
                    // }
                    // free(in_sessions);
                    // in_sessions = NULL;

                    recv(sockfd, &response, sizeof(message), 0);
                    if (strcmp((char*)response.data, "You have left all sessions.\n")==0){
                        sessionNum--;
                    }
                    printf("%s", response.data);

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
                if(arg > 2){
                    printf("You entered too many arguments, try again\n");
                    //continue;
                }
                else if(arg<2){
                    printf("You entered too few arguments, try again\n");
                    //continue;
                }
                else{
                    //printf("Sending message to create session %s\n",session_name);
                    message new_sess_request, response;
                    new_sess_request.type = NEW_SESS;
                    strcpy((char *)new_sess_request.source, username);
                    strcpy((char *)new_sess_request.data, session_name);
                    send(sockfd, &new_sess_request, sizeof(message), 0);
                    //in_session = (char *) malloc(MAX_NAME);
                    
                    recv(sockfd, &response, sizeof(message), 0);
                    if (strcmp((char*)response.data, "Created and joined the session.\n")==0){
                        sessionNum++;
                        printf("Created and joined the session. You are now in %d session(s).\n", sessionNum);
                    }else{
                        printf("%s", (char*)response.data);
                    }
                    //strcpy(in_sessions[sessionNum], session_name);
                }
            }
        }
        else if(strcmp(command, LIST_COMMAND) == 0){
            if(sockfd == -1){
                printf("You are not connected to a server yet!\n");
                fflush(stdout);
                //continue;
            }
            else{
                message list_request, response;
                list_request.type = QUERY;
                strcpy((char *)list_request.source, username);
                send(sockfd, &list_request, sizeof(message), 0);
                
                recv(sockfd, &response, sizeof(message), 0);
                printf("%s\n", response.data);
                fflush(stdout);
            }
        }

        else if(strcmp(command, INVITATION_COMMAND) == 0){
            message invitation, response;
            invitation.type = INVITATION;
            char *client_to_invite, *session, *extra_test;
            client_to_invite = strtok(NULL, SPACE);
            if(client_to_invite == NULL){
                printf("You entered too few arguments, try again\n");
            }
            else{
                session = strtok(NULL, SPACE);
                if(session == NULL){
                    printf("You entered too few arguments, try again\n");
                }
                else{
                    extra_test = strtok(NULL, SPACE);
                    if(extra_test != NULL){
                        printf("You entered too many arguments, try again\n");
                    }
                    else{
                        int loc = 0;
                        strcpy((char *)invitation.source, username);
                        strcpy((char *)(invitation.data+loc), client_to_invite);
                        loc+=strlen(client_to_invite);
                        invitation.data[loc] = ' ';
                        loc++;
                        strcpy((char *)(invitation.data+loc), session);
                        loc += strlen(session);
                        invitation.data[loc] = '\0';
                        loc++;
                        send(sockfd, &invitation, sizeof(message), 0);
                        //recv ack of invitation
                        recv(sockfd, &response, sizeof(message), 0);
                        printf("%s\n", response.data);


                    }
                }
            }
        }
        else if(strcmp(command, HELP_COMMAND) == 0){

        }


        else{
            //send plain text
            if(sockfd == -1){
                printf("You aren't connected to a server yet\n");
            }
            else if(sessionNum == -1){
                printf("You haven't joined any session yet\n");
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
                    printf("You said: %s\n", buffer);
                }
            }
        }
        if(sockfd != -1){
            fdmax = sockfd;
        }
        else{
            fdmax = STDIN;
        }
        buffer_size_temp = BUFFER_SIZE;
        fflush(stdin);
		memset(buffer, 0, 2000);	
        while(strlen(buffer) ==0){
            prompt_userinput(buffer, &buffer_size_temp, fdmax, username);
            fflush(stdout);
        }
        
        if(strcmp(buffer, QUIT_COMMAND) == 0 && sockfd != -1){
            message quit_request;
            if(sessionNum != -1){
                quit_request.type = LEAVE_ALL_SESS;
                strcpy((char *)quit_request.source, username);
                strcpy((char *)quit_request.data, ""); //Quits all sessions
                send(sockfd, &quit_request, sizeof(message), 0);
            }
            if(sockfd != -1){
                quit_request.type = EXIT;
                strcpy((char *)quit_request.source, username);
                send(sockfd, &quit_request, sizeof(message), 0);
                close(sockfd);
            }
        }
    }
    //free(in_session);
    free(username);
    free(buffer);
    return 0;
}