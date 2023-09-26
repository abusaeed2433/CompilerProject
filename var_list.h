#include<stdbool.h>

#ifndef VAR_LIST_H
#define VAR_LIST_H

struct VARIABLE{
    char name[200];
    double value;
    struct VARIABLE *prev;
    struct VARIABLE *next;
};

struct VARIABLE* createNode(const char *name, double value);
void insertVariable(char *name, double val);
void updateVariable(char *name, double val);
void deleteVariable(char *name);

bool doesVariableExists(char *name);
struct VARIABLE* getVariable(char* name);

void printAll();

#endif