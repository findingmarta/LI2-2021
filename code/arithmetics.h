/**
 * @file Este e o Header do ficheiro "arithmetics.c".
 */

#include "stack.h"

void isArithmeticsOp(STACK* s, char* op);

void arithmeticsOp(STACK* s, char* op);

void opUnaNum(STACK* s, char op);

void opUnaInt(STACK* s);

void opDuoNum(STACK* s, char op);

void opDuoInt(STACK* s, char op);
void opDuoInt1(char op, long* r1, long x1, long y1);
void opDuoInt2(char op, long* r1, long x1, long y1);
void opDuoInt3(char op, long* r1, long x1, long y1);
void opDuoInt4(char op, long* r1, long x1, long y1);