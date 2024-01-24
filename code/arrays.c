
#include "arrays.h"

#include "arithmetics.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Verifica se se trata de um operador de arrrays.
 *
 * Caso não se trate de um operador de arrays, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isArrayOp(STACK* s, char* op) {
    if (isArrayOpType(s, op))
        arrayOp(s, op);
    else
        isArithmeticsOp(s, op);
}
/**
 * @brief Verifica se se trata de uma operação de arrays de 2 operandos.
 *
 * Caso não se trate de operação de arrays de 2 operandos, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void arrayOp(STACK* s, char* op) {
    if (strlen(op) == 2)
        arrayTwoCharOp(s, op);
    else
        arrayOneCharOp(s, op[0]);
}
/**
 * @brief Verifica se se trata de um operador de arrrays.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 * @returns Valor lógico.
 */
int isArrayOpType(STACK* s, char* op) {
    SIMPLE_STACK* lastArr = getLastOpenArray(s);
    int isLastElemArray = isLastElemArrayCheck(lastArr);
    int isTwoLastElemArray = isTwoLastElemArrayCheck(lastArr);
    char* isZeroaryOp = strstr(" \"[],", op);
    int isUnaryOp = strstr(" ~,()S/N/", op) && isLastElemArray;
    int isBinaryOp = strstr(" +*=<>#/e>e<", op) && isTwoLastElemArray;
    return isZeroaryOp || isUnaryOp || isBinaryOp;
}
/**
 * @brief Verifica se o último elemento da stack se trata de um Array.
 *
 * @param s Stack.
 * @returns Valor lógico.
 */
int isLastElemArrayCheck(SIMPLE_STACK* s) {
    return top(s).type == ARRAY;
}
/**
 * @brief Verifica se os dois últimos elementos da stack se tratam de Arrays.
 *
 * @param s Stack.                        
 * @returns Valor lógico.
 */
int isTwoLastElemArrayCheck(SIMPLE_STACK* s) {
    return s->stack[s->n_elems - 2].type == ARRAY || isLastElemArrayCheck(s);
}
/**
 * @brief Encaminha para a operação de dois operandos correspondente ao operador.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void arrayTwoCharOp(STACK* s, char* op) {
    switch (op[0]) {
        case 'e':
            duoLogicArrOp(s, op[1]); // mudar isto
            break;
        case 'S':
            separateOps(s, op); // mudar isto
            break;
        case 'N':
            separateOps(s, op); // mudar isto
            break;
        default:
            break;
    }
}
/**
 * @brief Encaminha para a operação de um operando correspondente ao operador.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void arrayOneCharOp(STACK* s, char op) {
    switch (op) {
        case '[':
            createArray(s);
            break;
        case ']':
            closeArray(s);
            break;
        case ',':
            rangeOp(s);
            break;
        case '~':
            putArrElemsInStack(s);
            break;
        case '+':
            concatOps(s);
            break;
        case '*':
            replicateArrs(s);
            break;
        case '=':
            equalOp(s);
            break;
        case '>':
            greatersOp(s, 0);
            break;
        case '<':
            greatersOp(s, 1);
            break;
        case '(':
            removeOneElem(s, 1);
            break;
        case ')':
            removeOneElem(s, 0);
            break;
        case '#':
            strstrOp(s);
            break;
        case '/':
            splitBySubStrOp(s);
            break;
        default:
            break;
    }
}

// void zeroaryOp(STACK* s, char op) {}
/**
 * @brief Cria um Array e coloca-o na stack e na stack ligada.
 *
 * @param s Stack.
 */
void createArray(STACK* s) {
    DATA e = initArrElem(create_array());
    push(s, e);
    pushss(s->OpenArrayPtr, e);
}
/**
 * @brief Retira os elementos de um Array.
 *
 * @param s Stack.
 */
void closeArray(STACK* s) {
    if (s->OpenArrayPtr->n_elems > 1) popss(s->OpenArrayPtr);
}
/**
 * @brief Coloca os elementos de um Array numa Stack.
 *
 * @param s Stack.
 */
void putArrElemsInStack(STACK* s) {
    DATA e = pop(s);
    SIMPLE_STACK* ss = getArrValue(&e);
    for (int i = 0; i < ss->n_elems; i++) {
        push(s, ss->stack[i]);
    }
}
/**
 * @brief Efetua a operação de Tamanho ou Range de um Array.
 *
 * Verifica o comprimento de um Array e coloca esse valor na stack ou chama outra função.
 *
 * @param s Stack.
 */
void rangeOp(STACK* s) {
    DATA e = pop(s);
    if (e.type == 8) {
        DATA eTemp = e;
        storeData(&e, LONG, e.value.AP->n_elems);
        push(s, e);
        freeData(eTemp);
    } else
        rangeOp1(s, &e);
}
/**
 * @brief Cria um Array e coloca na stack a enumeração do seu intervalo.
 *
 * @param s Stack.
 * @param e Dado.
 */
void rangeOp1(STACK* s, DATA* e) {
    if (e->type < 8) {
        createArray(s);
        for (int i = 0; i < getIntegerValue(e); i++) {
            pushNumValue(s, LONG, i);
        }
        closeArray(s);
    }
}
/**
 * @brief Verifica qual de dois dados se trata de um Array, e qual se trata de um Integer.
 *
 * @param e1 Dado 1.
 * @param e2 Dado 2.
 * @param n Apontador que guarda o valor do Integer.
 * @returns O Array do dado.
 */
SIMPLE_STACK* orderArrayAndNumber(DATA e1, DATA e2, int* n) {
    SIMPLE_STACK* s;
    if (e1.type == ARRAY) {
        s = e1.value.AP;
        *n = getIntegerValue(&e2);
    } else {
        s = e2.value.AP;
        *n = getIntegerValue(&e1);
    }
    return s;
}
/**
 * @brief Efetua a operação de concatenar várias vezes Arrays.
 *
 * @param s Stack.
 */
void replicateArrs(STACK* s) {
    DATA e1 = pop(s), e2 = pop(s);
    int n;
    SIMPLE_STACK* ss = orderArrayAndNumber(e1, e2, &n);
    int n_elems = ss->n_elems;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n_elems; j++) {
            pushss(ss, ss->stack[j]);
        }
    }
    pushArrValue(s, ss);
}
/**
 * @brief Verifica se existe um array aberto, e copia-o para uma stack
 *
 * @param s Stack.
 * @param token Token.
 * @param size Tamanho do Array.
 *
 * @return 0 ou 1.
 */
int strWorker(STACK* s, char* token, int* size) {
    if (s->hasOpenStr) {
        strToStackAsArr(s, token);
        cleanStr(token, size);
        s->hasOpenStr = 0;
        return 1;
    } else {
        cleanStr(token, size);
        s->hasOpenStr = 1;
        return 0;
    }
}
/**
 * @brief Limpa um Array.
 *
 * @param str Apontador para String a limpar.
 * @param size Apontador para tamanho da string a limpar.
 */
void cleanStr(char* str, int* size) {
    for (int i = 0; i <= *size; i++) {
        str[i] = 0;
    }
    *size = 0;
}
/**
 * @brief Coloca uma string num array.
 *
 * @param s Stack.
 * @param str Apontador para string.
 */
void strToStackAsArr(STACK* s, char* str) {
    createArray(s);
    while (str[0] != '\0') {
        pushNumValue(s, CHAR, str[0]);
        str++;
    }
    closeArray(s);
}
/**
 * @brief Transforma um array de chars (stack de chars) numa string.
 *
 * @param e Dado.
 * @return String.
 */
char* chrArrToStr(DATA* e) {
    if (e->type == 8 /*Array*/) {
        SIMPLE_STACK* s = e->value.AP;
        int length = s->n_elems;
        char* r = malloc(sizeof(char) * length);
        cleanStr(r, &length);
        for (int i = 0; i < s->n_elems; i++) {
            r[i] = getIntegerValue(&(s->stack[i]));
        }
        return r;
    }
    return NULL;
}
/**
 * @brief Concatena duas strings.
 *
 * @param str1 string 1 a concatenar.
 * @param str2 string 2 a concatenar.
 * @returns String concatenada.
 */
char* concatString(char* str1, char* str2) {
    int strl1 = strlen(str1);
    str1 = realloc(str1, (strl1 + strlen(str2)) * sizeof(char));
    while (str2[0] != '\0') {
        str1[strl1] = str2[0];
        str2++;
        strl1++;
    }
    return str1;
}
/**
 * @brief Efetua a operação de buscar um valor por índice.
 *
 * @param s Stack.
 */
void equalOp(STACK* s) {
    DATA e1 = pop(s), e2 = pop(s);
    if (e1.type + e2.type == 16) {
        int r = areArraysEqual(e1.value.AP, e2.value.AP);
        pushNumValue(s, LONG, r);
    } else {
        push(s, valInIndexOp(e1, e2));
    }
}
/**
 * @brief Testa se dois Arrays são iguais.
 *
 * @param s1 Stack 1 a testar.
 * @param s2 Stack 2 a testar.
 * @returns 0 ou !0.
 */
int areArraysEqual(SIMPLE_STACK* s1, SIMPLE_STACK* s2) {
    int r = 0;
    if (s1->n_elems == s2->n_elems) {
        for (int i = 0; i < s1->n_elems; i++) {
            DATA e1 = s1->stack[i], e2 = s2->stack[i];
            TYPE t1 = e1.type, t2 = e2.type;
            if (t1 == t2) {
                if (t1 + t2 >= 8) {
                    r = areArraysEqual(e1.value.AP, e2.value.AP);
                } else
                    r = (getNumericValue(&e1) == getNumericValue(&e2));
            }
            if (r == 0) break;
        }
    }
    return r;
}
/**
 * @brief Efetua a operação de ir buscar X elementos do início ou fim.
 *
 * @param s Stack.
 * @param mode Decide se se trata da operação < ou >.
 */
void greatersOp(STACK* s, int mode) {
    DATA e1 = pop(s), e2 = pop(s);
    if (e1.type + e2.type == 16) {
        int r = 0;
        SIMPLE_STACK *s1 = e1.value.AP, *s2 = e2.value.AP;
        if (mode)
            r = (whichArrayIsBigger(s1, s2));
        else
            r = !(whichArrayIsBigger(s1, s2));
        pushNumValue(s, LONG, r);
    } else {
        createArray(s);
        valuesFromBegOrEnd(s, e1, e2, mode);
        closeArray(s);
    }
}
/**
 * @brief Averigua qual o array maior.
 *
 * @param s1 Stack com Array 1.
 * @param s2 Stack com Array 2.
 * @returns 0 ou !0.
 */
int whichArrayIsBigger(SIMPLE_STACK* s1, SIMPLE_STACK* s2) {
    int r = 0;
    for (int i = 0; i < s1->n_elems; i++) {
        DATA e1 = s1->stack[i], e2 = s2->stack[i];
        TYPE t1 = e1.type, t2 = e2.type;
        if (t1 == t2) {
            r = (getNumericValue(&e1) > getNumericValue(&e2));
        }
        if (r == 0) break;
    }
    return r;
}
/**
 * @brief Coloca dois dados numa stack, ordenando-os.
 *
 * @param e1 Dado 1.
 * @param e2 Dado 2.
 * @return A stack com os dados ordenados.
 */
DATA valInIndexOp(DATA e1, DATA e2) {
    int n;
    SIMPLE_STACK* ss = orderArrayAndNumber(e1, e2, &n);
    return ss->stack[n];
}
/**
 * @brief Decide se coloca os valores na stack a partir do fim ou do início.
 *
 * @param s Stack.
 * @param e1 Dado 1.
 * @param e2 Dado 2.
 * @param mode 0 ou 1.
 */
void valuesFromBegOrEnd(STACK* s, DATA e1, DATA e2, int mode) {
    int n;
    SIMPLE_STACK* ss = orderArrayAndNumber(e1, e2, &n);
    if (mode)
        pushValuesFromBeg(s, ss, n);
    else
        pushValuesFromEnd(s, ss, n);
}
/**
 * @brief Coloca na stack os valores de um Array a começar do início.
 *
 * @param s Stack.
 * @param ss Stack Array.
 * @param n Tamanho.
 */
void pushValuesFromBeg(STACK* s, SIMPLE_STACK* ss, int n) {
    for (int i = 0; i < n; i++) {
        push(s, ss->stack[i]);
    }
}
/**
 * @brief Coloca na stack os valores de um Array a começar do fim.
 *
 * @param s Stack.
 * @param ss Stack Array.
 * @param n Tamanho.
 */
void pushValuesFromEnd(STACK* s, SIMPLE_STACK* ss, int n) {
    int ne = ss->n_elems;
    for (int i = (ne - n); i < ne; i++) {
        push(s, ss->stack[i]);
    }
}
/**
 * @brief Efetua as operações correspondentes ao "e".
 *
 * @param s Stack.
 * @param op Operador.
 */
void duoLogicArrOp(STACK* s, char op) {
    int mode = 0;
    switch (op) {
        case '<':
            mode = 1;
            break;
        case '>':
            mode = 0;
        default:
            break;
    }
    DATA e1 = pop(s), e2 = pop(s);
    if (e1.type + e2.type == 16) {
        SIMPLE_STACK *s1 = e1.value.AP, *s2 = e2.value.AP, *r,
                     *be = (whichArrayIsBigger(s1, s2)) ? s1 : s2;
        if (mode)
            r = (be == s1) ? s2 : s1;
        else
            r = (be == s1) ? s1 : s2;
        pushArrValue(s, r);
        freeAllArraysInStack(s1);
        freeAllArraysInStack(s2);
    }
}
/**
 * @brief Efetua a operação de concatenar Arrays.
 *
 * @param s Stack.
 */
void concatOps(STACK* s) {
    DATA e1 = pop(s), e2 = pop(s);
    createArray(s);
    if (e2.type == ARRAY) {
        SIMPLE_STACK* ss = getArrValue(&e2);
        for (int i = 0; i < ss->n_elems; i++) {
            push(s, ss->stack[i]);
        }
        freeSimpleStack(ss);
    } else
        push(s, e2);
    if (e1.type == ARRAY) {
        SIMPLE_STACK* ss = getArrValue(&e1);
        for (int i = 0; i < ss->n_elems; i++) {
            push(s, ss->stack[i]);
        }
        freeSimpleStack(ss);
    } else
        push(s, e1);
    closeArray(s);
}
/**
 * @brief Efetua a operação de Remover 1º ou últ. elt. e colocar na stack após o array.
 *
 * @param s Stack.
 * @param mode Decide se se trata da operação ( ou ).
 */
void removeOneElem(STACK* s, int mode) {
    DATA e = pop(s), e1;
    createArray(s);
    SIMPLE_STACK* ss = e.value.AP;
    if (mode) {
        for (int i = 1; i < ss->n_elems; i++) {
            push(s, ss->stack[i]);
        }
        e1 = ss->stack[0];
    } else {
        for (int i = 0; i < ss->n_elems - 1; i++) {
            push(s, ss->stack[i]);
        }
        e1 = top(ss);
    }
    closeArray(s);
    push(s, e1);
    // freeSimpleStack(ss);
}
/**
 * @brief Efetua a operação de separar uma string por whitespace ou newline.
 *
 * @param s Stack.
 * @param op Operador.
 */
void separateOps(STACK* s, char* op) {
    if (strstr(" S/N/", op)) {
        char lim = (op[0] == 'S') ? ' ' : '\n';
        separateByOneChar(s, lim);
    }
}
/**
 * @brief Verifica se um Array possui um delimitador.
 *
 * @param s Stack.
 * @param s Stack.
 * @param lim Delimitador.
 * @param i Índice.
 */
void checkLimiter(STACK* s, SIMPLE_STACK* ss, char lim, int i) {
    DATA e = ss->stack[i];
    char chr = getIntegerValue(&e);
    int chk = (lim == ' ' && chr != '\n') || lim == '\n';
    if (lim != chr && chk)
        push(s, e);
    else {
        closeArray(s);
        createArray(s);
    }
}
/**
 * @brief Fecha um Array vazio.
 *
 * @param s Stack.
 */
void cleanEmptyArr(STACK* s) {
    SIMPLE_STACK* ss = pop(s).value.AP;
    createArray(s);
    for (int i = 0; i < ss->n_elems; i++) {
        DATA e = ss->stack[i];
        SIMPLE_STACK* s1 = e.value.AP;
        if (s1->n_elems != 0) {
            pushArrValue(s, s1);
        }
    }
    closeArray(s);
}
/**
 * @brief Separa um Array através de um dado um delimitador.
 *
 * @param s Stack.
 * @param lim Delimitador.
 */
void separateByOneChar(STACK* s, char lim) {
    SIMPLE_STACK* ss = pop(s).value.AP;
    createArray(s);
    createArray(s);
    for (int i = 0; i < ss->n_elems; i++) {
        checkLimiter(s, ss, lim, i);
    }
    closeArray(s);
    closeArray(s);
    if (lim == ' ') {
        cleanEmptyArr(s);
    }
}
/**
 * @brief Efetua a operação de procurar substring na string e devolver o índice.
 *
 * @param s Stack.
 */
void strstrOp(STACK* s) {
    DATA needleTest = pop(s);
    SIMPLE_STACK *needle, *haystack = pop(s).value.AP;
    if (needleTest.type == CHAR) {
        needle = create_array();
        pushss(needle, needleTest);
    } else
        needle = needleTest.value.AP;
    int r = findSubArrIndex(haystack, needle);
    pushNumValue(s, LONG, r);
    free(needle);
    free(haystack);
}
/**
 * @brief Retira o primeiro elemento de uma simple stack.
 *
 * @param ss Simple Stack.
 * @returns O último elemento.
 */
DATA begginingPop(SIMPLE_STACK* ss) {
    DATA e = ss->stack[0];
    ss->stack++;
    ss->n_elems--;
    return e;
}
/**
 * @brief Separa uma string em duas substrings.
 *
 * @param s Stack.
 * @param haystack String 1.
 * @param needle String 2.
 */
void splitBySubStrOpAux(STACK* s, SIMPLE_STACK* haystack, SIMPLE_STACK* needle) {
    int r = findSubArrIndex(haystack, needle);
    while (r > -1) {
        createArray(s);
        for (int i = 0; i < r; i++) {
            push(s, begginingPop(haystack));
        }
        for (int i = 0; i < needle->n_elems; i++) {
            begginingPop(haystack);
        }
        closeArray(s);
        r = findSubArrIndex(haystack, needle);
    }
}
/**
 * @brief Efetua a operação de separar string por substring.
 *
 * @param s Stack.
 */
void splitBySubStrOp(STACK* s) {
    SIMPLE_STACK *needle = pop(s).value.AP, *haystack = pop(s).value.AP;
    createArray(s);
    splitBySubStrOpAux(s, haystack, needle);
    if (haystack->n_elems != 0) {
        pushArrValue(s, haystack);
    }
    closeArray(s);
    freeSimpleStack(needle);
}
/**
 * @brief Procura o índice onde se encontra uma subArray.
 *
 * @param s1 Stack de procura.
 * @param s2 Stack a encontrar.
 */
int findSubArrIndex(SIMPLE_STACK* s1, SIMPLE_STACK* s2) {
    int s1Size = s1->n_elems, r = -1;
    s1->n_elems = (s2->n_elems);
    for (int i = 0; i <= s1Size - (s2->n_elems); i++) {
        s1->stack += i;
        if (areArraysEqual(s1, s2)) {
            r = i;
            s1->stack -= i;
            break;
        }
        s1->stack -= i;
    }
    s1->n_elems = s1Size;
    return r;
}
