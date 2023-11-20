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


void insertImport(char *imp);
bool isImportImported(char *imp);
void printAllImports();

void printAllLibraryFunction();
void initializeLibraryFunction();

double getLibrayFunctionResult(char* name, struct PARAMETER* params);

struct PARAMETER* createParameter(const char *type, double value);
void insertParameter( struct PARAMETER **head, struct PARAMETER **tail, char *type, double val);


struct PROTOTYPE* createProto(char* type, char* name,char* libraryName, struct PARAMETER*, struct PARAMETER* );
void insertProto(struct PROTOTYPE* var);
void insertLibraryProto(struct PROTOTYPE* var);


bool doesProtoExists(struct PROTOTYPE*, bool);
struct PROTOTYPE* getOriginalProto(struct PROTOTYPE*, bool);

void printAllProto();
void printProto(struct PROTOTYPE*, bool);

#endif
