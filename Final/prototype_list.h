#include<stdbool.h>

#ifndef PROTOTYPE_LIST_H
#define PROTOTYPE_LIST_H

struct PARAMETER{
    char type[10];
    struct PARAMETER *next;
    struct PARAMETER *prev;
};

struct PROTOTYPE{
    char funcType[10];
    char funcName[30];

    struct PARAMETER *paramsHead;
    struct PARAMETER *paramsTail;
    struct PROTOTYPE *prev;
    struct PROTOTYPE *next;
};


struct PARAMETER* createParameter(const char *type);
void insertParameter( struct PARAMETER **head, struct PARAMETER **tail, char *type);


struct PROTOTYPE* createProto(char*, char*, struct PARAMETER*, struct PARAMETER* );
void insertProto(struct PROTOTYPE* var);
bool doesProtoExists(struct PROTOTYPE* proto);
void printProto(struct PROTOTYPE *ptr);

#endif
