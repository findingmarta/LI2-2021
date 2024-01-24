/**
 * @file Este ficheiro contém as assinaturas das funções usadas em logics.c.
 */
#include "stack.h"

void isLogicsOp(STACK* s, char* op);

void logicsOp(STACK* s, char* op);

void singLogicOp(STACK* s, char op);

void evalCond(STACK* s, char op);

void notOp(STACK* s);

void evalCondBinario(STACK* s, char op);

void duoLogicOp(STACK* s, char op);

int traditionalLogicalOPS(DATA x, DATA y, char op);

int areElemsEqual(DATA* x, DATA* y);

void conditionalCtrl(STACK* s);

int isElemFalse(DATA e);

DATA falseElement();

DATA getBiggestElem(DATA x, DATA y);