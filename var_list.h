#include<stdbool.h>

#ifndef VAR_LIST_H
#define VAR_LIST_H

struct VARIABLE{
    char name[30];
    double value;
    char type[10];
    struct VARIABLE *prev;
    struct VARIABLE *next;
};

struct VARIABLE* createNode(const char *name, char *type, double value);
void insertVariable(char *name, char *type, double val);
void updateVariable(char *name, double val);
void deleteVariable(char *name);

bool doesVariableExists(char *name);
struct VARIABLE* getVariable(char* name);

void printAll();

#endif
