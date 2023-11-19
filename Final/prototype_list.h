#include<stdbool.h>

#ifndef PROTOTYPE_LIST_H
#define PROTOTYPE_LIST_H

struct PARAMETER{
    char type[10];
    double value;
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


struct PARAMETER* createParameter(const char *type, double value);
void insertParameter( struct PARAMETER **head, struct PARAMETER **tail, char *type, double val);


struct PROTOTYPE* createProto(char*, char*, struct PARAMETER*, struct PARAMETER* );
void insertProto(struct PROTOTYPE* var);
bool doesProtoExists(struct PROTOTYPE* proto);

struct PROTOTYPE* getOriginalProto(struct PROTOTYPE*);

void printAllProto();
void printProto(struct PROTOTYPE*, bool);

#endif
