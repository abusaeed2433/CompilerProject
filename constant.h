
#include<stdbool.h>
#ifndef CONSTANT_H
#define CONSTANT_H

bool isCondOpValid(char* op);
bool isConditionValid(double left, char* op, double right);

double getValue(double left, double right, char *op);
void processStatement(const char *stmt, char *leftVar, char *varBeforeOp, char *operator, char *varAfterOp);

#endif
