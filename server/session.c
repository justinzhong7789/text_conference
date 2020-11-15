#include "session.h"
//struct sessionNode sessionList[MAXSECTIONS]; //list of all sessions, in main function
//int curSize = 0;

//Insert to the list
int insertSession(struct sessionNode** sessionList, int* listSize, struct sessionNode* newSession){
	if (*listSize<MAXNUMSESSIONS){
		sessionList[*listSize]=newSession;
		*listSize=*listSize+1;
		return 1;
	} else{
		printf("Session list is full.\n");
		return 0;
	}
}

//Find session index, given session name
int findSessionByName(struct sessionNode** sessionList, int* listSize, char* thisSessionName){
	for (int i=0; i<*listSize; i++){
		if(strcmp(sessionList[i]->sessionName, thisSessionName)==0){
			return i;
		}
	}
	return -1;
}

//Delete session, given a socket number
int deleteSession(struct sessionNode** sessionList, int* listSize, int thisSockfd){
	for (int i=0; i<*listSize; i++){
		if (sessionList[i]->sockfd==thisSockfd){
			free(sessionList[i]); //Don't need to increment count
			return 1;
		}
	}
	return 0;
}

void printAllSessions(struct sessionNode** sessionList, int* listSize){
	for (int i = 0; i<*listSize; i++){
		assert(sessionList[i]);
		printf("Session Name: %s\n",sessionList[i]->sessionName);
		printf("Session IP Address: %s\n", sessionList[i]->IPAddress);
		printf("Session Port: %d\n", sessionList[i]->port);
		printf("Session sockfd: %d\n", sessionList[i]->sockfd);
	}
	return;
}