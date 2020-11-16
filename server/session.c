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
	return -1;
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
	assert(sessionList[sessionIdx]);
	assert(strcmp(sessionList[sessionIdx]->clientIDs[clientIdx], "")!=0); //ensure client ID is not null
	memset(sessionList[sessionIdx]->clientIDs[clientIdx],0,MAXNUMCLIENTS);//Clear the entry
	//Shift the rest of the client IDs forward 
	for(int j=clientIdx; j<sessionList[sessionIdx]->curNumClients-1; j++){
		strcpy(sessionList[sessionIdx]->clientIDs[j], sessionList[sessionIdx]->clientIDs[j+1]);
	}
	//Decrement the number of clients
	sessionList[sessionIdx]->curNumClients--;
}

//Delete session, given a socket number
void deleteSession(struct sessionNode** sessionList, int* listSize, int sessionIdx){
	printf("Deleting session with index %d\n", sessionIdx);
	//Free the session node
	free(sessionList[sessionIdx]->sockfds);
	for(int n=0; n<MAXNUMCLIENTS; n++){
		free(sessionList[sessionIdx]->clientIDs[n]);
	}
	free(sessionList[sessionIdx]->clientIDs);
	free(sessionList[sessionIdx]);

	//Move the rest of sessions forward
	for (int i=sessionIdx; i<*listSize-1; i++){
		sessionList[i]=sessionList[i+1];
	}

	//Decrement the number of sessions
	*listSize = *listSize - 1;	
}

//Delete all sessions
void deleteAllSessions(struct sessionNode** sessionList){
	printf("Deleting all sessions\n");
	for (int i=0; i<MAXNUMSESSIONS; i++){
		if (sessionList[i]!=NULL){
			free(sessionList[i]->sockfds);
			for(int n=0; n<MAXNUMCLIENTS; n++){
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
		printf("Session client IDs: ");
		for (int j=0; j<sessionList[i]->curNumClients; j++){
			printf("%s ", sessionList[i]->clientIDs[j]);
		}
		printf(".\n");

		printf("Session sockfd: ");
		for (int k=0; k<sessionList[i]->curNumClients; k++){
			printf("%d ", sessionList[i]->sockfds[k]);
		}
		printf(".\n");
	}
}