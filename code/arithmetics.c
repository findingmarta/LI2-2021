/**
 * @file Este ficheiro contém funções auxiliares.
 */

#include "arithmetics.h"

#include "manipulations.h"
#include "stack.h"

#include <math.h>
#include <string.h>

/**
 * @brief Verifica se se trata de um operador aritmético.
 *
 * Caso não se trate de um operador aritmético, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isArithmeticsOp(STACK* s, char* op) {
    if (strstr(" +-*/()#%&^~|", op))
        arithmeticsOp(s, op);
    else
        isManipulationsOp(s, op);
}

/**
 * @brief Verifica a qual operação aritmética corresponde o token.
 *
 * Verifica a operação e quantidade e tipo de operandos necessários para a
 * efetuar.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void arithmeticsOp(STACK* s, char* op) {
    char oper = op[0];
    if (strstr(" +-*/#", op))
        opDuoNum(s, oper);
    else if (strstr(" ()", op))
        opUnaNum(s, oper);
    else if (strstr(" %&^|", op))
        opDuoInt(s, oper);
    else if (strstr(" ~", op))
        opUnaInt(s);
}

/**
 * @brief Efetua as operações unárias de operadores DOUBLE.
 *
 * Efetua as operações que utilizam apenas um operador do tipo DOUBLE e coloca
 * na stack.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void opUnaNum(STACK* s, char op) {
    DATA x = pop(s), r;
    double r1 = 0, x1 = getNumericValue(&x);
    switch (op) {
        case '(':
            r1 = x1 - 1;
            break;
        case ')':
            r1 = x1 + 1;
            break;
        default:
            break;
    }
    storeData(&r, x.type, r1);
    push(s, r);
}

/**
 * @brief Efetua as operações unárias de operadores LONG.
 *
 * Efetua as operações que utilizam apenas um operador do tipo LONG e coloca na
 * stack.
 *
 * @param s Stack.
 */
void opUnaInt(STACK* s) {
    DATA x = pop(s), r;
    long r1 = -1 - getIntegerValue(&x);
    storeData(&r, x.type, r1);
    push(s, r);
}

/**
 * @brief Efetua as operações binárias de operadores DOUBLE.
 *
 * Efetua as operações que utilizam dois operadores do tipo DOUBLE e coloca na
 * stack.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void opDuoNum(STACK* s, char op) {
    DATA x = pop(s), y = pop(s), r;
    double r1 = 0, x1 = getNumericValue(&x), y1 = getNumericValue(&y);
    switch (op) {
        case '+':
            r1 = y1 + x1;
            break;
        case '-':
            r1 = y1 - x1;
            break;
        case '*':
            r1 = y1 * x1;
            break;
        case '/':
            r1 = y1 / x1;
            break;
        case '#':
            r1 = pow(y1, x1);
            break;
        default:
            break;
    }
    storeData(&r, getResultingType(x, y), r1);
    push(s, r);
}

/**
 * @brief Efetua as operações binárias de operadores LONG.
 *
 * Efetua as operações que utilizam dois operadores do tipo LONG e coloca na
 * stack.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void opDuoInt(STACK* s, char op) {
    DATA x = pop(s), y = pop(s), r;
    long r1 = 0, x1 = getIntegerValue(&x), y1 = getIntegerValue(&y);
    opDuoInt1(op, &r1, x1, y1);
    storeData(&r, getResultingType(x, y), r1);
    push(s, r);
}

/**
 * @brief Procedimento auxiliar de opDuoInt.
 *
 * Efetua as operações relacionadas com o operador '%'.
 * Caso o operador não seja '%' avança para o próximo procedimento.
 *
 * @param op Símbolo do operador.
 * @param r1 Resultado da operação.
 * @param x1 Valor.
 * @param y1 Valor.
 */
void opDuoInt1(char op, long* r1, long x1, long y1) {
    if (op == '%')
        *r1 = y1 % x1;
    else
        opDuoInt2(op, r1, x1, y1);
}

/**
 * @brief Procedimento auxiliar de opDuoInt.
 *
 * Efetua as operações relacionadas com o operador '&'.
 * Caso o operador não seja '&' avança para o próximo procedimento.
 *
 * @param op Símbolo do operador.
 * @param r1 Resultado da operação.
 * @param x1 Valor.
 * @param y1 Valor.
 */
void opDuoInt2(char op, long* r1, long x1, long y1) {
    if (op == '&')
        *r1 = x1 & y1;
    else
        opDuoInt3(op, r1, x1, y1);
}

/**
 * @brief Procedimento auxiliar de opDuoInt.
 *
 * Efetua as operações relacionadas com o operador '^'.
 * Caso o operador não seja '^' avança para o próximo procedimento.
 *
 * @param op Símbolo do operador.
 * @param r1 Resultado da operação.
 * @param x1 Valor.
 * @param y1 Valor.
 */
void opDuoInt3(char op, long* r1, long x1, long y1) {
    if (op == '^')
        *r1 = x1 ^ y1;
    else
        opDuoInt4(op, r1, x1, y1);
}

/**
 * @brief Procedimento auxiliar de opDuoInt.
 *
 * Efetua as operações relacionadas com o operador '|'.
 *
 * @param op Símbolo do operador.
 * @param r1 Resultado da operação.
 * @param x1 Valor.
 * @param y1 Valor.
 */
void opDuoInt4(char op, long* r1, long x1, long y1) {
    if (op == '|') *r1 = x1 | y1;
}