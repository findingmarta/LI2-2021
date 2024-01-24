#include "logics.h"

#include "convertions.h"
#include "stack.h"
#include "string.h"
#include "variables.h"

#include <stdio.h>

/**
 * @brief Verifica se se trata de um operador lógico.
 *
 * Caso não se trate de um operador lógico, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isLogicsOp(STACK* s, char* op) {
    if (strstr(" =<>!e&e|e<e>?", op))
        logicsOp(s, op);
    else
        isConvertOp(s, op);
}

/**
 * @brief Verifica o numero de caracteres do operador logico e reencaminha o operador para a
 * respetiva funcao.
 *
 *
 * @param s Stack.
 * @param op Operador especifico a ser executado.
 */
void logicsOp(STACK* s, char* op) {
    if (strlen(op) == 1) {
        singLogicOp(s, op[0]);
    } else
        duoLogicOp(s, op[1]);
}

/**
 * @brief Trata de verificar que funcao a ser executada para os operadores logicos de um caracter.
 *
 * Usada para separar a funcao do operador if-then-else dos outros
 * operadores de um caracter.
 *
 * @param s Stack.
 * @param op Operador especifico a ser executado.
 */
void singLogicOp(STACK* s, char op) {
    if (op == '?') {
        conditionalCtrl(s);
    } else
        evalCond(s, op);
}

/**
 * @brief Trata de verificar que funcao a ser executada para os operadores logicos de um caracter.
 *
 * Usada para separar a funcao do operador Não dos outros
 * operadores de um caracter.
 *
 * @param s Stack.
 * @param op Operador especifico a ser executado.
 */
void evalCond(STACK* s, char op) {
    if (op == '!')
        notOp(s);
    else
        evalCondBinario(s, op);
}

/**
 * @brief Funcao que executa o operador Não.
 *
 * Efetua a operação e coloca o resultado na stack.
 *
 * @param s Stack.
 */
void notOp(STACK* s) {
    DATA e = pop(s);
    storeData(&e, LONG, isElemFalse(e));
    push(s, e);
}

/**
 * @brief Trata da execucao dos operadores logicos de um caracter.
 *
 * Efetua a operação correspondente e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param op Operador especifico a ser executado.
 */
void evalCondBinario(STACK* s, char op) {
    DATA x = pop(s), y = pop(s), r;
    double x1 = getNumericValue(&x), y1 = getNumericValue(&y);
    long r1 = 0;
    switch (op) {
        case '=':
            r1 = y1 == x1;
            break;
        case '<':
            r1 = y1 < x1;
            break;
        case '>':
            r1 = y1 > x1;
            break;
        default:
            break;
    }
    storeData(&r, LONG, r1);
    push(s, r);
}

/**
 * @brief Funcao que executa o operador if-then-else.
 *
 * Efetua a operação e coloca o resultado na stack.
 *
 * @param s Stack.
 */
void conditionalCtrl(STACK* s) {
    DATA resElse = pop(s);
    DATA resThen = pop(s);
    DATA condition = pop(s);
    if (!isElemFalse(condition))
        push(s, resThen);
    else
        push(s, resElse);
}

/**
 * @brief Trata da execucao dos operadores logicos de dois caracteres.
 *
 * Efetua a operação correspondente e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param op Operador especifico a ser executado.
 */
void duoLogicOp(STACK* s, char op) {
    DATA x = pop(s), y = pop(s), r, be = getBiggestElem(x, y);
    if ((op == '&') || (op == '|'))
        r = traditionalLogicalOPS(x, y, op) ? be : falseElement();
    else
        switch (op) {
            case '<':
                r = areElemsEqual(&x, &be) ? y : x;
                break;
            case '>':
                r = be;
                break;
            default:
                break;
        }
    push(s, r);
}

/**
 * @brief Funcao que executa os operadores lógicos "e" ou "ou".
 *
 * Efetua a operação de acordo com o token.
 *
 * @param x Dado.
 * @param y Dado.
 * @param op Operador a ser executado.
 * @returns Inteiro correspondente ao resultado de cada operação.
 */
int traditionalLogicalOPS(DATA x, DATA y, char op) {
    int xv = !isElemFalse(x), yv = !isElemFalse(y);
    if (op == '&')
        return (xv && yv);
    else if (op == '|')
        return (xv || yv);
    return 0;
}

/**
 * @brief Funcao que executa os operadores lógicos "e" ou "ou".
 *
 * Efetua a operação de acordo com o token.
 *
 * @param x Dado.
 * @param y Dado.
 * @returns Valor lógico.
 */
int areElemsEqual(DATA* x, DATA* y) {
    return (getNumericValue(x) == getNumericValue(y) && (x->type == y->type));
}

/**
 * @brief Trata da execucao dos operadores logicos com shortcut.
 *
 * Efetua a operação correspondente ao token.
 *
 * @param x Stack. FIX THIS
 */
void eComShortcutOp(int x);

/**
 * @brief Testa se um valor é falso ou um DATA vazio.
 *
 * @param e Valor a testar.
 * @returns 0 ou !0.
 */
int isElemFalse(DATA e) {
    TYPE t = e.type;
    return (
        (t < 8 && getNumericValue(&e) == 0) || t == EMPTY ||
        (t == ARRAY && e.value.AP->n_elems == 0));
}

/**
 * @brief Verifica o maior de dois valores
 *
 * @param x Valor a testar.
 * @param y Segundo valor a testar.
 * @returns O maior valor.
 */
DATA getBiggestElem(DATA x, DATA y) {
    return (getNumericValue(&x) > getNumericValue(&y)) ? x : y;
}

/**
 * @brief Cria um elemento do tipo DATA que e falso
 *
 * @returns O elemento que é falso.
 */
DATA falseElement() {
    DATA r;
    storeData(&r, LONG, 0);
    return r;
}