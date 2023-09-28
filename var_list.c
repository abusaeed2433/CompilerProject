#include<stdio.h>
#include <stddef.h>
#include<string.h>
#include<stdlib.h>
#include "var_list.h"
#include <stdbool.h>



struct VARIABLE *head = NULL;
struct VARIABLE *tail = NULL;

struct VARIABLE* createNode(const char *name, double value) {
    struct VARIABLE *newNode = (struct VARIABLE*) malloc(sizeof(struct VARIABLE));
    if(!newNode) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->value = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insertVariable(char *name, double val) {
    struct VARIABLE *var = createNode(name,val);
    
    if(tail == NULL) {
        head = var;
        tail = var;
    }
    else {
        var->prev = tail;
        tail->next = var;
        tail = var;
    }
}

void updateVariable(char *name, double val){
    struct VARIABLE *ptr;
    ptr = head;
    while(ptr != NULL){
        if( strcmp(ptr->name,name) == 0 ){
            ptr->value = val;
            break;
        }
        ptr = ptr->next;
    }
}

void deleteVariable(char *name){
    struct VARIABLE *ptr;
    ptr = head;
    while(ptr != NULL){
        if( strcmp(ptr->name, name) == 0 ){
            // first delete
            if(ptr == head){
                //single node
                if(ptr == tail) tail == NULL;
                head = ptr->next;
            }
            else if(ptr == tail){ // last delete
                tail = tail->prev;
                tail->next = NULL;
            }
            else{
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;
            }
        }
        ptr = ptr->next;
    }
}

bool doesVariableExists(char *name){
    struct VARIABLE *ptr = head;

    while (ptr != NULL)
    {
        if( strcmp(name,ptr->name) == 0 ) return true;
        ptr = ptr->next;
    }
    return false;
}

struct VARIABLE* getVariable(char* name){
    //printf("printing from inner\n");
    //printAll();
    struct VARIABLE *ptr = head;
    while (ptr != NULL)
    {
        //printf("(%s %ld),",ptr->name,ptr->value);
        if( strcmp(name,ptr->name) == 0 ){
            //printf("\n value returning %s %lf\n",ptr->name,ptr->value);
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void printAll(){
    if(head == NULL) return;

    struct VARIABLE *ptr;
    ptr = head;
    while(ptr != NULL){
        printf("%s %lf -> ",ptr->name,ptr->value);
        ptr = ptr->next;
    }
    printf("\n");
}
