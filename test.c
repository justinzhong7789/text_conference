#include <ctype.h>
#include "server/server.h"
#include "client/client.h"


typedef struct node {
    char *name;
    int id;
    struct node *next;
} node;

int main(int argc, char ** argv){
    size_t buf_size = 2000;
    unsigned char cpy[1000];
    char *buffer = (char *)malloc(buf_size);
    getline(&buffer, &buf_size, stdin);
    if(buffer[strlen(buffer)] == '\n'){
        buffer[strlen(buffer)] = '\0';
    }
    strcpy((char *)cpy, buffer);
    printf("%s",cpy);

    return 0;
}

