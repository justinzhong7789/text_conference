#include <ctype.h>
#include "server/server.h"
#include "client/client.h"
#include <time.h>

typedef struct node {
    char *name;
    int id;
    struct node *next;
} node;

int main(int argc, char ** argv){
    clock_t t = clock();
    for(int i=0;i< 100000;i++);
    t = clock() - t;
    printf("%d", (int)t);
    return 0;
}

