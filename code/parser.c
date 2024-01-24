/**
 * @file Este ficheiro contêm as funções relacionadas ao parser.
 */

#include "parser.h"

#include "arithmetics.h"
#include "arrays.h"
#include "blocks.h"
#include "variables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Esta é a função que vai fazer o parse de uma linha.
 *
 * @param line Corresponde à linha que foi lida e da qual se vai fazer o parse.
 * @param s Corresponde à Stack.
 * @returns Stack.
 */
STACK* parse(char* line, STACK* s) {
    parseLoop(s, line);
    return s;
}

/**
 * \brief Esta é a função que vai fazer o parse de uma linha.
 *
 * @param s Stack.
 * @param line Corresponde à linha que foi lida e da qual se vai fazer o parse.
 */
void parseLoop(STACK* s, char* line) {
    char* token = malloc(sizeof(char) * strlen(line));
    int j = strlen(line) - 1;
    cleanStr(token, &j);
    while (strlen(line)) {
        if (line[0] == '{') {
            if (!s->hasOpenBlock) {
                line += 2;
            }
            s->hasOpenBlock++;
        }
        if (line[0] == '}' && s->hasOpenBlock && !s->hasOpenStr) {
            if (blockBuilder(s, token, &j))
                line++;
            else
                s->hasOpenBlock--;
        }
        if (line[0] == '\"' && !s->hasOpenBlock)
            if (!strWorker(s, token, &j)) line++;
        if (evalChar(s, line[0])) {
            evalToken(s, token, &j);
            line++;
            continue;
        }
        token[j] = line[0];
        line++;
        j++;
    }
    free(token);
}

/**
 * \brief Esta é a função que vai avaliar um char na stack.
 *
 * @param s Stack.
 * @param chr Corresponde ao char a ser avaliado.
 * @returns Valor lógico.
 */
int evalChar(STACK* s, char chr) {
    return !(s->hasOpenStr) && !(s->hasOpenBlock) && (chr == ' ' || chr == '\n');
}

/**
 * \brief Esta é a função que vai avaliar uma string token na stack.
 *
 * @param s Stack.
 * @param token Corresponde a token.
 * @param j Corresponde ao tamanho de token.
 */
void evalToken(STACK* s, char* token, int* j) {
    // printf("TOKEN= %s\n", token);
    token = evalNumber(s, token);
    if (token[0] != '\0') {
        isVarsOp(s, token);
    }
    cleanStr(token, j);
}

/**
 * \brief Esta é a função que vai avaliar um número na stack.
 *
 * @param s Stack.
 * @param token Corresponde a uma string token.
 * @returns token Token.
 */
char* evalNumber(STACK* s, char* token) {
    char* sobra;
    DATA val_i;
    int isNegative = 0;
    if (token[0] == '-' && strlen(token) > 1) {
        isNegative = 1;
        token++;
    }
    long valorl = strtol(token, &sobra, 10);
    storeData(&val_i, LONG, valorl);
    if (sobra[0] == '.') sobra = evalDecimalNumber(sobra, &val_i);
    if (isNegative) {
        TYPE t = val_i.type;
        double v = 0 - getNumericValue(&val_i);
        storeData(&val_i, t, v);
    }
    if ((strlen(sobra) == 0 && strlen(token) != 0) || sobra[0] == '\n') push(s, val_i);
    return token;
}

/**
 * \brief Esta é a função que vai avaliar um número decimal.
 *
 * @param sobra Resto da string que sobrou.
 * @param val_i Corresponde a um dado.
 * @returns sobra Resto da string.
 */
char* evalDecimalNumber(char* sobra, DATA* val_i) {
    sobra++;
    char* token = sobra;
    sobra = "";
    double valord = getNumericValue(val_i);
    long tmp = strtol(token, &sobra, 10);
    double decimas = transfNumeroParaDecimas(tmp);
    valord = valord + decimas;
    storeData(val_i, DOUBLE, valord);
    return sobra;
}

/**
 * \brief Esta função transforma um número inteiro em décimas.
 *
 * @param n Número a ser transformado.
 * @returns n O número transformado 
 */
double transfNumeroParaDecimas(double n) {
    while ((int) n != 0) {
        n = n * 0.1;
    }
    return n;
}
