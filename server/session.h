/* Operations on Session linked list
*/

#include <stdio.h>
#ifndef SESSION_H
#define SESSION_H

#include <stdlib.h>



#define MAXSOCKETS 128
#define MAXSECTIONS 128

typedef struct sessionNode{
    int sessionName;
    char *session_id;
    int socket_id[MAXSOCKETS];
}sessionNode;

// struct sessionNode sessionList[MAXSECTIONS]; //list of all sessions
// int curSize = 0;

int insertSession(struct sessionNode* newSession);
int findSessionOfClient(char* clientID);
int deleteSession(struct sessionNode* newSession);
void printAllSessions();

#endif

