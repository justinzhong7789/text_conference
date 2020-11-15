/* Operations on Session linked list
*/

#include <stdio.h>
#ifndef SESSION_H
#define SESSION_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXNUMSESSIONS 128
#define MAXNUMCLIENTS 10

struct sessionNode{
    char* sessionName;
    char* IPAddress;
    int port;
    int sockfd;
    char* clientIDs[MAXNUMCLIENTS];
};


int insertSession(struct sessionNode** sessionList, int* listSize, struct sessionNode* newSession);
int findSessionByName(struct sessionNode** sessionList, int* listSize, char* thisSessionName);
int findSessionOfClient(struct sessionNode** sessionList, int* listSize, char* clientID);
int deleteSession(struct sessionNode** sessionList, int* listSize, int thisSockfd);
void deleteAllSessions(struct sessionNode** sessionList);
void printAllSessions(struct sessionNode** sessionList, int* listSize);

#endif

