
#include "variables.h"

#include "arrays.h"
#include "blocks.h"
#include "stack.h"

#include <string.h>

/**
 * @brief Verifica se se trata de uma operação de variáveis.
 *
 * Caso não se trate de uuma operação de variáveis, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isVarsOp(STACK* s, char* op) {
    if (strstr(" :A:B:C:D:E:F:G:H:I:j:K:L:M:N:O:P:Q:R:S:T:U:V:W:X:Y:Z", op))
        varsOp(s, op);
    else
        isBlockOp(s, op);
}

/**
 * @brief Realiza as operações relacionadas com as variáveis.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void varsOp(STACK* s, char* op) {
    int isPushVar = (op[0] != ':');
    char var = isPushVar ? op[0] : op[1];
    int ind = getVarIndex(var);
    if (isPushVar) {
        push(s, getVar(s, ind));
    } else {
        s->vars[ind] = cloneDATA(topPointer(s));
    }
}

/**
 * @brief Encontra o índice da variável recebida.
 *
 * @param x Variável.
 * @returns x-65 Índice da variável.
 */
int getVarIndex(char x) {
    return x - 65;
}

/**
 * \brief Esta é o procedimento que vai fazer o armazenamento de uma variável.
 *
 * @param s Stack.
 * @param index Corresponde ao índice da variável .
 * @param t Corresponde ao tipo desse elemento.
 * @param v Valor.
 */
void storeVar(STACK* s, int index, TYPE t, double v) {
    DATA e;
    storeData(&e, t, v);
    s->vars[index] = e;
}

/**
 * \brief Esta função vai adquirir uma variável.
 *
 * @param s Stack.
 * @param index Corresponde ao índice da variável .
 * @returns Variável.
 */
DATA getVar(STACK* s, int index) {
    return cloneDATA(&s->vars[index]);
}