/**
 * @file Este ficheiro contém funções auxiliares.
 */

#include "manipulations.h"

#include "logics.h"
#include "stack.h"

#include <string.h>

/**
 * @brief Verifica se se trata de um operador de manipulação.
 *
 * Caso não se trate de um operador de manipulação, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isManipulationsOp(STACK* s, char* op) {
    if (strstr(" _;\\@$", op))
        manipulationsOp(s, op);
    else
        isLogicsOp(s, op);
}

/**
 * @brief Verifica a qual operação de manipulação corresponde o token.
 *
 * Verifica a operação e quantidade de operandos necessários para a efetuar.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void manipulationsOp(STACK* s, char* op) {
    if (strstr(" _;$", op))
        manipulationsOpOne(s, *op);
    else
        manipulationsOpNotOne(s, op[0]);
}

/**
 * @brief Verifica a qual operação de manipulação corresponde o token.
 *
 * Verifica a operação a efetuar.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void manipulationsOpNotOne(STACK* s, char op) {
    if (op == '\\')
        manipulationsOpTwo(s);
    else if (op == '@')
        manipulationsOpThree(s);
}

/**
 * @brief Efetua as operações de manipulação da stack que utilizam três valores.
 *
 * Efetua a manipulação correspondente ao token e coloca o resultado na stack.
 *
 * operador @
 *
 * @param s Stack.
 */
void manipulationsOpThree(STACK* s) {
    DATA x = pop(s), y1 = pop(s), z = pop(s);
    push(s, y1);
    push(s, x);
    push(s, z);
}

/**
 * @brief Efetua as operações de manipulação da stack que utilizam dois valores.
 *
 * Efetua a manipulação correspondente ao token e coloca o resultado na stack.
 *
 * operador \
 *
 * @param s Stack.
 */
void manipulationsOpTwo(STACK* s) {
    DATA y = pop(s), x = pop(s);
    push(s, y);
    push(s, x);
}

/**
 * @brief Trata de verificar que função de manipulação unária deve ser executada.
 *
 * Usada para separar as operações de manipulação que utilizam um operando
 *
 * @param s Stack.
 * @param op Operador especifico a ser executado.
 */
void manipulationsOpOne(STACK* s, char op) {
    DATA x = pop(s);
    switch (op) {
        case '_':
            manipulationsOpUnderscore(s, x);
            break;
        case '$':
            manipulationsOpDolarSign(s, x);
            break;
        default:
            break;
    }
}

/**
 * @brief Funcao que executa o operador Underscore.
 *
 * Efetua a operação de duplicar e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param x Operando
 */
void manipulationsOpUnderscore(STACK* s, DATA x) {
    push(s, x);
    push(s, x);
}

/**
 * @brief Funcao que executa o operador dolar.
 *
 * Efetua a operação que copia n-ésimo elemento para o topo da stack e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param x Operando
 */
void manipulationsOpDolarSign(STACK* s, DATA x) {
    push(s, getElemAtPosFromStack(s->ss, getIntegerValue(&x)));
}