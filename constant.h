
#include<stdbool.h>
#ifndef CONSTANT_H
#define CONSTANT_H

// true if op is a valid conditional operator
bool isCondOpValid(char* op);

// true if condition is valid after performing operation on given parameter
bool isConditionValid(double left, char* op, double right);

// perform arithmatic operations and returns result
double getValue(double left, double right, char *op);

// process statement(a = b add c) and return result in leftVar, varBeforeOp, operator, varAfterOp
void processStatement(const char *stmt, char *leftVar, char *varBeforeOp, char *operator, char *varAfterOp);

// trim given string
char* trim(char *s);

#endif
