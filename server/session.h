/* Operations on Session linked list
*/

#include <stdio.h>
#ifndef SESSION_H
#define SESSION_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXNUMSESSIONS 128

struct sessionNode{
    char* sessionName;
    char* IPAddress;
    int port;
    int sockfd;
    //int socket_id[MAXSOCKETS];
};


int insertSession(struct sessionNode** sessionList, int* listSize, struct sessionNode* newSession);
int findSessionOfClient(struct sessionNode** sessionList, int* listSize, char* clientID);
int deleteSession(struct sessionNode** sessionList, int* listSize, int thisSockfd);
void printAllSessions(struct sessionNode** sessionList, int* listSize);

#endif

