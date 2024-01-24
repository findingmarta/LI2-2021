
#include "inputoutput.h"

#include "arrays.h"
#include "parser.h"
#include "stack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Verifica se se trata de uma operação de Input/Output.
 *
 * Caso não se trate de uma operação de Input/Output, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isInputOutputOp(STACK* s, char* op) {
    if (strstr(" ltp", op)) inputOutputOp(s, op[0]);
    // else
    // isArrayOp(s, op);
}

/**
 * @brief Efetua as operações de Input/Output.
 *
 * Efetua a manipulação correspondente ao token.
 *
 * @param s Stack.
 * @param op Token.
 */
void inputOutputOp(STACK* s, char op) {
    switch (op) {
        case 'l':
            readOneLineOp(s);
            break;
        case 't':
            readMultiplesLines(s);
            break;
        default:
            break;
    }
}

/**
 * @brief Efetua a operação de ler uma linha.
 *
 * @param s Stack.
 */
void readOneLineOp(STACK* s) {
    char* linha = readLine();
    strToStackAsArr(s, linha);
    free(linha);
}

/**
 * @brief Função auxiliar para a leitura de uma linha.
 *
 * Aloca memória e guarda o conteúdo de uma linha.
 *
 * @returns A linha.
 */
char* readLine() {
    char* linha = malloc(sizeof(char) * 10420);
    assert(fgets(linha, 10420, stdin) != NULL);
    return linha;
}

/**
 * @brief Função auxiliar para a leitura de várias linhas.
 *
 * @param s Stack.
 */
void readMultiplesLinesAux(STACK* s) {
    SIMPLE_STACK* ss = pop(s).value.AP;
    if (top(ss).value.CHAR == '\n') {
        // popss(ss);
        // DATA e;
        // storeData(&e, CHAR, '\0');
        // pushss(ss, e);
    }
    pushArrValue(s, ss);
}

/**
 * @brief Função que trata da leitura de várias linhas.
 *
 * Aloca memória e guarda o conteúdo de cada linha.
 *
 * @param s Stack.
 */
void readMultiplesLines(STACK* s) {
    char* linha = malloc(sizeof(char) * 10240);
    createArray(s);
    closeArray(s);
    while (fgets(linha, 10240, stdin) != NULL) {
        strToStackAsArr(s, linha);
        concatOps(s);
    }
    DATA empty;
    empty.type = LONG;
    empty.value.LONG = 1;
    pushss(top(s->ss).value.AP, empty);
    readMultiplesLinesAux(s);
    free(linha);
}

/**
 * @brief Efetua o push dos elementos de uma stack para outra.
 *
 * @param subj Stack origem.
 * @param rs Stack destino.
 */
void pushStackElemsToOtherStack(STACK* subj, STACK* rs) {
    SIMPLE_STACK* ss = subj->ss;
    for (int i = 0; i < ss->n_elems; i++) {
        DATA newElem = ss->stack[i];
        push(rs, newElem);
    }
}
