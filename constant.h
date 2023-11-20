
#include<stdbool.h>
#ifndef CONSTANT_H
#define CONSTANT_H

//returns true if ch is an arithmatic operator
bool isArithOp(char *ch);

// true if op is a valid conditional operator
bool isCondOpValid(char* op);

// true if condition is valid after performing operation on given parameter
bool isConditionValid(double left, char* op, double right);

// perform arithmatic operations and returns result
double getValue(double left, double right, char *op);

#endif