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
			printf("Found match on index %d", i);
			return i;
		}
	}
	return -1;
}

//Return session index, given client ID
int* findSessionsOfClient(struct sessionNode** sessionList, int* listSize, char* clientID){
	int result[1];
	int length = 1; 
	
	for (int i=0; i<*listSize; i++){
		for (int j=0; j<MAXNUMCLIENTS; j++){
			if(strcmp(sessionList[i]->clientIDs[j], clientID)==0){
				result = realloc(result, )
				result[length]=i;
				//return i;
			}
		}
	}
	return -1;
}

//Return the index of client, given client ID and its session
int findIdxOfClient(struct sessionNode** sessionList, char* clientID, int sessionIdx){
	printf("Looking for index of client ID within cliend ID list.\n");
	assert(sessionList[sessionIdx]);
	for (int j = 0; j<MAXNUMCLIENTS; j++){
		if(strcmp(sessionList[sessionIdx]->clientIDs[j], clientID)==0){
			printf("Found client ID match at index %d.\n", j);
			return j;
		}
	}
	return -1;//not found
}

//Remove client ID, and shift the rest of clients forward in the list
void removeClientID(struct sessionNode** sessionList, int clientIdx, int sessionIdx){
	printf("Removing Client ID %d from session %d.\n", clientIdx, sessionIdx);
	assert(sessionList[sessionIdx]!=NULL);
	//assert(strcmp(sessionList[sessionIdx]->clientIDs[clientIdx], "")!=0); //ensure client ID is not empty
	strcpy(sessionList[sessionIdx]->clientIDs[clientIdx],"");//Clear the entry
	//Shift the rest of the client IDs forward 
	for(int j=clientIdx; j<MAXNUMCLIENTS-1; j++){
		strcpy(sessionList[sessionIdx]->clientIDs[j], sessionList[sessionIdx]->clientIDs[j+1]);
	}
	//Decrement the number of clients
	sessionList[sessionIdx]->curNumClients = sessionList[sessionIdx]->curNumClients - 1;
}

//Delete session, given a session index number
void deleteSession(struct sessionNode** sessionList, int* listSize, int sessionIdx){
	printf("Deleting session with index %d, list size%d\n", sessionIdx, *listSize);
	//Free the session node
	free(sessionList[sessionIdx]->sessionName);
	free(sessionList[sessionIdx]->sockfds);
	for(int n=0; n<MAXNUMCLIENTS; n++){
		free(sessionList[sessionIdx]->clientIDs[n]);
	}
	free(sessionList[sessionIdx]->clientIDs);
	//free(sessionList[sessionIdx]);

	//Move the rest of sessions forward
	for (int i=sessionIdx; i<MAXNUMSESSIONS-1; i++){
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
			free(sessionList[i]->sessionName);
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
	printf("Printing all sessions, list size %d\n", *listSize);
	for (int i = 0; i<*listSize; i++){
		if(sessionList[i]!=NULL){
			printf("Session Name: %s\n",sessionList[i]->sessionName);
			printf("Session IP Address: %s\n", sessionList[i]->IPAddress);
			printf("Current number of clients: %d\n", sessionList[i]->curNumClients);
			printf("Session client IDs: ");
			for (int j=0; j<sessionList[i]->curNumClients; j++){
				printf("%s ", sessionList[i]->clientIDs[j]);
			}
			printf("\n");

			printf("Session sockfd: ");
			for (int k=0; k<sessionList[i]->curNumClients; k++){
				printf("%d ", sessionList[i]->sockfds[k]);
			}
			printf(".\n");
		}
	}
}