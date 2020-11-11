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


char DATABASE[] = "database.txt"; 
int CHAR_SIZE = 1;



//write a new line to databasefile (create this file if it does not exist), in the formate
// username password
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
    FILE *dbfile = fopen(DATABASE, "w");
    size_t buf_size = 50;
    char buffer[buf_size];
    while (!feof(dbfile))
    {   
        getline(&buffer, &buf_size, dbfile);
        char delim = ' ';
        char *token = strtok(buffer, delim);
        if(strcmp(token, username) ==0){
            token = strtok(NULL, delim);
            if(strcmp(token, inputpw)==0){
                return 0;
            }
            else{
                return -1;
            }
        }
    }
    return -2;

}