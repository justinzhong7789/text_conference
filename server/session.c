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
		return -1;
	}
}

//Return session index, given session name
int findSessionByName(struct sessionNode** sessionList, int* listSize, char* thisSessionName){
	for (int i=0; i<*listSize; i++){
		if(strcmp(sessionList[i]->sessionName, thisSessionName)==0){
			return i;
		}
	}
	return -1;
}

//Return session index, given client ID
int findSessionOfClient(struct sessionNode** sessionList, int* listSize, char* clientID){
	for (int i=0; i<*listSize; i++){
		for (int j=0; j<MAXNUMCLIENTS; j++){
			if(strcmp(sessionList[i]->clientIDs[j], clientID)==0){
				return i;
			}
		}
	}
	return 0;
}

//Return the index of client, given client ID and its session
int findIdxOfClient(struct sessionNode** sessionList, char* clientID, int sessionIdx){
	assert(sessionList[sessionIdx]);
	for (int j = 0; j<MAXNUMCLIENTS; j++){
		if(strcmp(sessionList[sessionIdx]->clientIDs[j], clientID)==0){
			return j;
		}
	}
	return -1;//not found
}

//Remove client ID, and shift the rest of clients forward in the list
void removeClientID(struct sessionNode** sessionList, int clientIdx, int sessionIdx){
	//memset(sessionList[sessionIdx]->clientIDs,0,strlen(cX));

	assert(sessionList[sessionIdx]);
	assert(strcmp(sessionList[sessionIdx]->clientIDs[clientIdx], "")!=0); //ensure client ID is not null
	

}

//Delete session, given a socket number
void deleteSession(struct sessionNode** sessionList, int sessionIdx){
	printf("Deleting specified session\n");
	
}

//Delete all sessions
void deleteAllSessions(struct sessionNode** sessionList){
	printf("Deleting all sessions\n");
	for (int i=0; i<MAXNUMSESSIONS; i++){
		if (sessionList[i]!=NULL){
			for(int n=0; n<MAXSIZECLIENTID; n++){
				free(sessionList[i]->clientIDs[n]);
			}
			free(sessionList[i]->clientIDs);
			free(sessionList[i]);
			sessionList[i]=NULL;
		}
	}
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