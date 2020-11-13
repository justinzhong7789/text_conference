#include <ctype.h>
#include "server/server.h"
#include "client/client.h"


typedef struct node {
    char *name;
    int id;
    struct node *next;
} node;


node* create_node (char *name_, int id_){
    node *new_one = (node *)malloc(sizeof(node));
    new_one->id = id_;
    new_one->next = NULL;
    new_one->name = name_;
    return new_one;
}

void insert(node **list_head, node* newNode){
    if(*list_head == NULL){
        *list_head = newNode;
    }
    else {
        node *t;
        t = *list_head;
        while (t->next!=NULL)
        {
            t = t->next;
        }
        t->next = newNode;
    }
}

void freehead(node *head){
    node *t, *p;
    t = head;
    while(t!=NULL){
        p = t;
        t = t->next;
        free(p);
    }
}


int main(int argc, char ** argv){

    node **list = (node **)malloc(sizeof(node **));
    *list = NULL;
    for(int i = 0;i<5;i++){
        node *newnode = create_node(NULL, i);
        insert(list, newnode);
    }
    char a[] = "sadadasas";
    (*list)->name = (char *)malloc(50);
    strcpy((*list)->name, a);
    printf("%d" , (*list)->id);
    printf("%s", (*list)->name);


    freehead(*list);

    return 0;
}

