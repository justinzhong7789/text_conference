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


void writeToDatabase(char *username, char *password);
int checkPW(char *username, char *inputpw);

char DATABASE[] = "database.txt"; 
int CHAR_SIZE = 1;



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