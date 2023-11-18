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

// create variable node using passed parameter
struct VARIABLE* createNode(const char *name, char *type, double value);

// insert variable into linked-list
void insertVariable(char *name, char *type, double val);

// update variable with given val
void updateVariable(char *name, double val);

// delete variable from linked-list
void deleteVariable(char *name);

// return total number of variables
int getTotalVar();

// return true if varible exists with passed name
bool doesVariableExists(char *name);

// return variable struct of given name or NULL if not found
struct VARIABLE* getVariable(char* name);

double getValueOrDefault(char* name);

char* getFormattedValueOrDefault(char *name);

// print all variable available when called
void printAll();

#endif
