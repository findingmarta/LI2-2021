/**
 * @file Este e o Header do ficheiro "variables.c".
 */
#include "stack.h"

void isVarsOp(STACK* s, char* op);

void varsOp(STACK* s, char* op);

int getVarIndex(char x);

void storeVar(STACK* s, int index, TYPE t, double v);

DATA getVar(STACK* s, int index);