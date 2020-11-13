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
    char buffer[BUFFER_SIZE];

    prompt_userinput(buffer, &BUFFER_SIZE);
    while(buffer != QUIT_COMMAND){
        int arg = 0;
        char *command;
        command = strtok(buffer, SPACE);
        arg++;
        
        //LOGIN = request to establish connection + checking password on server side
        if(command == LOGIN_COMMAND){
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
                login_request(ID, PW, S_IP, PORT);


            }

        }
        else if(command == LOGOUT_COMMAND){

        }
        else if(command == JOIN_SESSION_COMMAND){
            char *session_name = strtok(NULL, SPACE);
        }
        else if(command == LEAVE_SESSION_COMMAND){


        }
        else if(command == CREATE_SESSION_COMMAND){
            char *session_name = strtok(NULL, SPACE);

        }
        else if(command == LIST_COMMAND){


        }

        else{
            printf("Command is not found\n");
        }


        prompt_userinput(buffer, &BUFFER_SIZE);
    }

    free(buffer);

    return 0;
}