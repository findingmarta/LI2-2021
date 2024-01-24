/**
 * @file Este ficheiro contém funções auxiliares.
 */

#include "stack.h"

void isManipulationsOp(STACK* s, char* op);

void manipulationsOp(STACK* s, char* op);

void manipulationsOpNotOne(STACK* s, char op);

/**
 * @brief Efetua as operações de manipulação da stack que utilizam um valor.
 *
 * Efetua a manipulação correspondente ao token e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param op Token.
 */
void manipulationsOpOne(STACK* s, char op);

/**
 * @brief Efetua as operações de manipulação da stack que utilizam dois valores.
 *
 * Efetua a manipulação correspondente ao token e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param op Token.
 */
void manipulationsOpTwo(STACK* s);

/**
 * @brief Efetua as operações de manipulação da stack que utilizam três valores.
 *
 * Efetua a manipulação correspondente ao token e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param op Token.
 */
void manipulationsOpThree(STACK* s);

void manipulationsOpUnderscore(STACK* s, DATA x);

void manipulationsOpDolarSign(STACK* s, DATA x);