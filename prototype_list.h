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
    char libraryName[30];

    struct PARAMETER *paramsHead;
    struct PARAMETER *paramsTail;
    struct PROTOTYPE *prev;
    struct PROTOTYPE *next;
};

void insertImport(char fulImp[20]);

bool isImportImported(char *imp);

void printAllImports();

struct PARAMETER* createParameter(const char *type,double value);

void insertParameter( struct PARAMETER **head, struct PARAMETER **tail, char *type, double val);

struct PROTOTYPE* createProto(char *type, char *name, char *libraryName, struct PARAMETER *paramsHead, struct PARAMETER *paramsTail );

void insertLibraryProto(struct PROTOTYPE* var);

void printAllLibraryFunction();

void insertProto(struct PROTOTYPE* var);

struct PROTOTYPE* getOriginalProto(struct PROTOTYPE* proto, bool isLibrary);

bool doesProtoExists(struct PROTOTYPE* proto, bool isLibrary);

void printAllProto();

// prints data also if reverse is false
void printProto(struct PROTOTYPE *ptr, bool reverse);

double getLibrayFunctionResult(char* name, struct PARAMETER* params);

void initializeLibraryFunction();

#endif