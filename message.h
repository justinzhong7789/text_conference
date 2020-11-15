#pragma once

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

#define MAX_NAME 100
#define MAX_DATA 1000

typedef struct message {
    unsigned int type;
    unsigned int size;
    unsigned char source[MAX_NAME];
    unsigned char data[MAX_DATA];
} message;



enum type{
    LOGIN = 0,
    LO_ACK = 1,
    LO_NAK = 2,
    EXIT = 3,
    JOIN = 4,
    JN_ACK = 5,
    JN_NAK = 6,
    LEAVE_SESS = 7,
    NEW_SESS = 8,
    NS_ACK = 9,
    MESSAGE =10,
    QUERY = 11,
    QU_ACK = 12
};

void print_t(int a);

void print_t(int a){
	for(int i=0;i<a;i++){
		printf("\t");
	}
}
