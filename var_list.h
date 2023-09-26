#ifndef VAR_LIST_H
#define VAR_LIST_H

void insert(char* name, double val);
#include<stdio.h>
#include <stddef.h>
#include<string.h>
#include<stdlib.h>
#include "var_list.h"

struct VARIABLE* createNode(const char *name, double value);
void insertVariable(char *name, double val);
void updateVariable(char *name, double val);
void deleteVariable(char *name);
void printAll();

#endif
