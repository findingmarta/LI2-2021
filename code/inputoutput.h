/**
 * @file Este contém as funções relacionadas a "inputoutput.c".
 */
#include "stack.h"

void isInputOutputOp(STACK* s, char* op);

void inputOutputOp(STACK* s, char op);

void readOneLineOp(STACK* s);

void readMultiplesLines(STACK* s);

void pushStackElemsToOtherStack(STACK* subj, STACK* rs);
char* readLine();