/**
 * @file Este ficheiro contém a função principal do parse.
 */

#ifndef PARSER_H
#define PARSER_H

#include "stack.h"

STACK* parse(char* line, STACK* s);

void parseLoop(STACK* s, char* line);

int evalChar(STACK* s, char chr);

void evalToken(STACK* s, char* token, int* j);

char* evalNumber(STACK* s, char* token);

char* evalDecimalNumber(char* sobra, DATA* val_i);

double transfNumeroParaDecimas(double n);

void cleanStr(char* str, int* size);

#endif