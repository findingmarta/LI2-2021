
#include "convertions.h"

#include "arrays.h"
#include "inputoutput.h"
#include "parser.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Verifica se se trata de uma operação de conversão.
 *
 * Caso não se trate de uma operação de conversão, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isConvertOp(STACK* s, char* op) {
    if (strstr(" ifcs", op))
        convertOp(s, op[0]);
    else
        isInputOutputOp(s, op);
}

/**
 * @brief Funcção principal para efetuar as operações de conversão.
 *
 * Efetua a operação correspondente e coloca o resultado na stack.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void convertOp(STACK* s, char op) {
    DATA r, val = pop(s);
    if (val.type == ARRAY) {
        char* linha = chrArrToStr(&val);
        if (op != 'f') {
            evalNumber(s, linha);
        } else {
            pushNumValue(s, DOUBLE, strtod(linha, NULL));
        }
        free(val.value.AP);
    } else if (op == 's') {
        char* text = malloc(sizeof(char) * 10240);
        sprintf(text, "%g", getNumericValue(&val));
        strToStackAsArr(s, text);
        free(text);
    } else {
        r = convertDataToType(val, op);
        push(s, r);
    }
}
// t S/ { i } % { e> } *
/**
 * @brief Função auxiliar para efetuar as operações de conversão.
 *
 * Efetua a operação correspondente ao operador.
 *
 * @param e Corresponde ao elemento da stack ao qual será aplicado a conversão.
 * @param op Símbolo do operador.
 * @returns O valor e convertido.
 */
DATA convertDataToType(DATA e, char op) {
    tryConvertDataToDouble(&e, op);
    return e;
}

/**
 * @brief Função auxiliar para converter um dado para Double.
 *
 * Caso não consiga converter para Double tenta converter para Char.
 *
 * @param e Corresponde ao elemento da stack ao qual será aplicado a conversão.
 * @param op Símbolo do operador.
 */
void tryConvertDataToDouble(DATA* e, char op) {
    if (op == 'f')
        storeData(e, DOUBLE, getNumericValue(e));
    else
        tryConvertDataToChar(e, op);
}

/**
 * @brief Função auxiliar para converter um dado para Char.
 *
 * Caso não consiga converter para Double tenta converter para Long.
 *
 * @param e Corresponde ao elemento da stack ao qual será aplicado a conversão.
 * @param op Símbolo do operador.
 */
void tryConvertDataToChar(DATA* e, char op) {
    if (op == 'c')
        storeData(e, CHAR, labs(getIntegerValue(e)));
    else
        tryConvertDataToLong(e, op);
}

/**
 * @brief Função auxiliar para converter um dado para Long.
 *
 * Armazena o dado convertido.
 *
 * @param e Corresponde ao elemento da stack ao qual será aplicado a conversão.
 * @param op Símbolo do operador.
 */
void tryConvertDataToLong(DATA* e, char op) {
    if (op == 'i') {
        if (e->type == CHAR) {
            storeData(e, LONG, e->value.CHAR);
        } /* else if (e->type == ARRAY) {
            char *text = chrArrToStr(e), *restus;
            long valorl = strtol(text, &restus, 10);
            storeData(e, LONG, valorl);
        } */
        else
            storeData(e, LONG, getNumericValue(e));
    }
}