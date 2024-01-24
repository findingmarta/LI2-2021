

#ifndef ARRAYS_H
#define ARRAYS_H

#include "stack.h"

void isArrayOp(STACK* s, char* op);
void arrayOp(STACK* s, char* op);

int isArrayOpType(STACK* s, char* op);
int isLastElemArrayCheck(SIMPLE_STACK* s);
int isTwoLastElemArrayCheck(SIMPLE_STACK* s);

void arrayTwoCharOp(STACK* s, char* op);
void arrayOneCharOp(STACK* s, char op);

void createArray(STACK* s);
void closeArray(STACK* s);

void rangeOp(STACK* s);
void rangeOp1(STACK* s, DATA* e);

void putArrElemsInStack(STACK* s);

void replicateArrs(STACK* s);
SIMPLE_STACK* orderArrayAndNumber(DATA e1, DATA e2, int* n);

int strWorker(STACK* s, char* token, int* size);
void cleanStr(char* str, int* size);
void strToStackAsArr(STACK* s, char* str);
char* chrArrToStr(DATA* e);
char* concatString(char* str1, char* str2);

void equalOp(STACK* s);
int areArraysEqual(SIMPLE_STACK* s1, SIMPLE_STACK* s2);

int whichArrayIsBigger(SIMPLE_STACK* s1, SIMPLE_STACK* s2);
DATA valInIndexOp(DATA e1, DATA e2);

void greatersOp(STACK* s, int mode);
void valuesFromBegOrEnd(STACK* s, DATA e1, DATA e2, int mode);
void pushValuesFromBeg(STACK* s, SIMPLE_STACK* ss, int n);
void pushValuesFromEnd(STACK* s, SIMPLE_STACK* ss, int n);

void duoLogicArrOp(STACK* s, char op);
void concatOps(STACK* s);

void removeOneElem(STACK* s, int mode);

void separateOps(STACK* s, char* op);
void separateByOneChar(STACK* s, char lim);

void strstrOp(STACK* s);
void splitBySubStrOp(STACK* s);
int findSubArrIndex(SIMPLE_STACK* s1, SIMPLE_STACK* s2);

#endif