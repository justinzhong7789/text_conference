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


char DATABASE[] = "database.txt"; 
int CHAR_SIZE = 1;



//write a new line to databasefile (create this file if it does not exist), in the formate
// username password
void writeToDatabase(char *username, char *password){
    FILE *dbfile = fopen(DATABASE, "a+");
    fwrite(username, CHAR_SIZE, strlen(username), dbfile);
    fputc(' ', dbfile);
    fwrite(password, CHAR_SIZE, strlen(password), dbfile);
    fputc('\n', dbfile);
    fclose(dbfile);
}