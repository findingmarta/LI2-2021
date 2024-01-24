/**
 * @file Este e o Header do ficheiro "convertions.c".
 */

#include "stack.h"

void isConvertOp(STACK* s, char* op);
void convertOp(STACK* s, char op);
DATA convertDataToType(DATA e, char op);

void tryConvertDataToDouble(DATA* e, char op);
void tryConvertDataToChar(DATA* e, char op);
void tryConvertDataToLong(DATA* e, char op);