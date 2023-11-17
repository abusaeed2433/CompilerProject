#include<stdio.h>
#include <stddef.h>
#include<string.h>
#include<stdlib.h>
#include "prototype_list.h"
#include <stdbool.h>

struct PROTOTYPE *protoHead = NULL;
struct PROTOTYPE *protoTail = NULL;

struct PARAMETER* createParameter(const char *type) {

    struct PARAMETER *node = (struct PARAMETER*) malloc(sizeof(struct PARAMETER));

    if(!node) { printf("Memory allocation failed.\n"); return NULL; }

    strncpy(node->type, type, sizeof(node->type) - 1);
    
    node->prev = NULL;
    node->next = NULL;
    return node;
}



void insertParameter( struct PARAMETER **head, struct PARAMETER **tail, char *type){
    
    struct PARAMETER *var = createParameter(type);

    if(*tail == NULL) {
        *head = var;
        *tail = var;
    }
    else {
        var->prev = (*tail);
        (*tail)->next = var;
        (*tail) = var;
    }
}


struct PROTOTYPE* createProto(char *type, char *name, 
        struct PARAMETER *paramsHead, struct PARAMETER *paramsTail ) {

    struct PROTOTYPE *newNode = (struct PROTOTYPE*) malloc(sizeof(struct PROTOTYPE));
    if(!newNode) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    //printf("Inside-3 %s \n",type);
    strncpy(newNode->funcType, type, sizeof(newNode->funcType) - 1);
    
    //printf("Inside-4\n");
    strncpy(newNode->funcName, name ,sizeof(newNode->funcName)-1);
    
    //printf("Inside-5\n");
    newNode->paramsHead = paramsHead;
    newNode->paramsTail = paramsTail;

    newNode->prev = NULL;
    newNode->next = NULL;

    //printf("Inside - 6\n");
    return newNode;
}

void insertProto(struct PROTOTYPE* var) {
    
    if(protoTail == NULL) {
        protoHead = var;
        protoTail = var;
    }
    else {
        var->prev = protoTail;
        protoTail->next = var;
        protoTail = var;
    }
}

bool doesProtoExists(struct PROTOTYPE* proto){

    struct PROTOTYPE *ptr = protoHead;
    while (ptr != NULL)
    {
        bool isNameSame = strcmp( proto->funcName ,ptr->funcName) == 0 ? true : false;

        if(isNameSame){

            struct PARAMETER *param1 = proto->paramsTail;
            struct PARAMETER *param2 = ptr->paramsTail;

            while ( param1 != NULL && param2 != NULL )
            {
                if( strcmp(param1->type, param2->type)  != 0){
                    break;
                }

                param1 = param1->prev;
                param2 = param2->prev;
            }

            if(param1 == NULL && param2 == NULL){
                return true;
            }

        }

        ptr = ptr->next;
    }
    return false;
}

void printProto(struct PROTOTYPE *ptr){

    printf("\nPrototype found: | %s %s( ",ptr->funcType, ptr->funcName);

    struct PARAMETER* param = ptr->paramsTail;

    while(param != NULL){
        printf("%s ",param->type);
        param = param->prev;
    }
    printf("); |\n");

}
