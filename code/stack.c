/**
 * @file Este ficheiro contêm as funções que inicializam e alteram a stack.
 */

#include "stack.h"

#include "variables.h"

#define _POSIX_C_SOURCE 200809L ///< Permite aceder a outras funções.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Esta é a função que vai fazer o armazenamento de um dado.
 *
 * @param e Corresponde ao pointer do elemento da stack.
 * @param t Corresponde ao tipo desse elemento.
 * @param v Valor.
 */
void storeData(DATA* e, TYPE t, double v) {
    tryStoreDataD(e, t, v);
}

/**
 * @brief Esta função atribui um valor a um DATA.
 *
 * Atribui o valor indicado a um DATA, caso este seja do tipo Double.
 *
 * @param e Um qualquer DATA.
 * @param t Tipo do DATA.
 * @param v Valor a atribuir.
 */
void tryStoreDataD(DATA* e, TYPE t, double v) {
    if (t == DOUBLE) {
        e->value.DOUBLE = v;
        e->type = t;
    } else
        tryStoreDataL(e, t, v);
}

/**
 * @brief Esta função atribui um valor a um DATA.
 *
 * Atribui o valor indicado a um DATA, caso este seja do tipo Long.
 *
 * @param e Um qualquer DATA.
 * @param t Tipo do DATA.
 * @param v Valor a atribuir.
 */
void tryStoreDataL(DATA* e, TYPE t, double v) {
    if (t == LONG) {
        e->value.LONG = v;
        e->type = t;
    } else
        tryStoreDataC(e, t, v);
}

/**
 * @brief Esta função atribui um valor a um DATA.
 *
 * Atribui o valor indicado a um DATA, caso este seja do tipo Char.
 *
 * @param e Um qualquer DATA.
 * @param t Tipo do DATA.
 * @param v Valor a atribuir.
 */
void tryStoreDataC(DATA* e, TYPE t, double v) {
    if (t == CHAR) {
        e->value.CHAR = v;
        e->type = t;
    }
}

#define alocMemSS (SIMPLE_STACK*) malloc(sizeof(SIMPLE_STACK)) ///< Aloca memória para uma stack.

/**
 * @brief Cria uma stack.
 *
 * @returns s Stack.
 */
STACK* create_stack() {
    STACK* s = (STACK*) malloc(sizeof(STACK));
    initStackSubstacks(s);

    s->hasOpenStr = 0;
    s->hasOpenBlock = 0;
    return s;
}
/**
 * @brief Aloca memória para uma stack e substacks.
 *
 * @param s Stack.
 */
void initStackSubstacks(STACK* s) {
    s->ss = alocMemSS;
    s->OpenArrayPtr = alocMemSS;
    initStackArr(s->ss, 10);
    initStackArr(s->OpenArrayPtr, 4);
    pushss(s->OpenArrayPtr, initArrElem(s->ss));
}

/**
 * @brief Inicializa uma stack, alocando memória.
 *
 * @param s Stack.
 * @param size Tamanho da Stack.
 */
void initStackArr(SIMPLE_STACK* s, int size) {
    s->n_elems = 0;
    s->size = size;
    s->stack = (DATA*) calloc(s->size, sizeof(DATA));
}

/**
 * @brief Inicializa as variáveis da stack.
 *
 * @param s Stack.
 */
void initVarsArr(STACK* s) {
    s->vars = malloc(26 * sizeof(DATA));
    initVarsArrP1(s);
    initVarsArrP2(s);
    initVarsArrP3(s);
}

/**
 * @brief Inicializa as variáveis da stack do tipo Long.
 *
 * @param s Stack.
 */
void initVarsArrP1(STACK* s) {
    for (int i = 0; i < 6; i++) {
        storeVar(s, i, LONG, i + 10);
    }
}
/**
 * @brief Inicializa as variáveis da stack do tipo Char.
 *
 * @param s Stack.
 */
void initVarsArrP2(STACK* s) {
    storeVar(s, 13, CHAR, '\n');
    storeVar(s, 18, CHAR, ' ');
}
/**
 * @brief Inicializa as variáveis da stack do tipo Long.
 *
 * @param s Stack.
 */
void initVarsArrP3(STACK* s) {
    for (int i = 0; i < 3; i++) {
        storeVar(s, 23 + i, LONG, i);
    }
}

/**
 * @brief Retira um elemento da stack.
 *
 * @param s Stack.
 * @returns s Decrementa o valor de n_elems e retorna o elemento cujo indice é o
 * último elemento de topo.
 */
DATA pop(STACK* s) {
    return (popss(LastOpenArray));
}
/**
 * @brief Retira um elemento de uma simple stack.
 *
 * @param s Simple Stack.
 * @returns s Decrementa o valor de n_elems e retorna o elemento cujo indice é o
 * último elemento de topo.
 */
DATA popss(SIMPLE_STACK* s) {
    // needsLessMem(s);
    DATA r;
    if (!is_empty(s)) {
        s->n_elems--;
        r = s->stack[s->n_elems];
    } else {
        r.type = EMPTY;
    }
    return r;
}

/**
 * @brief Adiciona um elemento à stack.
 *
 * @param s Stack.
 * @param elem Elemento a adicionar.
 */
void push(STACK* s, DATA elem) {
    pushss((LastOpenArray), elem);
}
/**
 * @brief Introduz um valor num dado, conforme o seu tipo e coloca o dado na stack.
 *
 * @param s Stack.
 * @param t Tipo do dado
 * @param v Valor a introduzir
 */
void pushNumValue(STACK* s, TYPE t, double v) {
    DATA e;
    storeData(&e, t, v);
    push(s, e);
}
/**
 * @brief Coloca na stack o valor de um dado de um Array.
 *
 * @param s Stack.
 * @param ss Simple Stack
 */
void pushArrValue(STACK* s, SIMPLE_STACK* ss) {
    DATA e = initArrElem(ss);
    push(s, e);
}

/**
 * @brief Adiciona um elemento à stack.
 *
 * Incrementa um valor e realoca a stack.
 * Adiciona o elemento elem ao elemento ao topo da stack.
 *
 * @param s Stack simples.
 * @param elem Elemento a adicionar.
 */
void pushss(SIMPLE_STACK* s, DATA elem) {
    if (elem.type != EMPTY) {
        needsMoreMem(s);
        s->stack[s->n_elems] = elem;
        s->n_elems++;
    }
}

/**
 * @brief Esta função aloca mais memória numa stack.
 *
 * @param s Stack simples.
 */
void needsMoreMem(SIMPLE_STACK* s) {
    if (s->size == s->n_elems) {
        s->size *= 2;
        s->stack = (DATA*) realloc(s->stack, s->size * sizeof(DATA));
    }
}

/**
 * @brief Esta função aloca menos memória numa stack.
 *
 * @param s Stack simples.
 */
void needsLessMem(SIMPLE_STACK* s) {
    if (s->size * 0.3 >= s->n_elems) {
        s->size = s->n_elems + ((int) (s->size * 0.1)) + 1;
        s->stack = (DATA*) realloc(s->stack, s->size * sizeof(DATA));
    }
}

/**
 * @brief Liberta toda a memória de uma stack previamente alocada.
 *
 * @param s Stack.
 */
void freeStack(STACK* s) {
    SIMPLE_STACK* ss = s->ss;
    freeSimpleStack(ss);
    free(s->vars);
    free(s->OpenArrayPtr);
    free(s);
}
/**
 * @brief Liberta a memória de um dado de acordo com o seu tipo.
 *
 * @param e Dado.
 */
void freeData(DATA e) {
    switch (e.type) {
        case ARRAY:
            /* array */
            freeSimpleStack(e.value.AP);
            break;
        case BLOCK:
            /* block */
            free(e.value.BLOCK);
            break;
        default:
            break;
    }
}

/**
 * @brief Liberta toda a memória de uma simple stack.
 *
 * @param ss Simple Stack.
 */
void freeSimpleStack(SIMPLE_STACK* ss) {
    freeAllArraysInStack(ss);
    free(ss);
}
/**
 * @brief Liberta toda a memória de Arrays numa stack.
 *
 * @param s Stack.
 */
void freeAllArraysInStack(SIMPLE_STACK* s) {
    for (int i = 0; i < s->n_elems; i++) {
        DATA s1 = s->stack[i];
        freeData(s1);
    }
}

/**
 * @brief Verifica o elemento no topo da stack.
 *
 * @param s Stack simples.
 * @returns s Elemento no topo da stack.
 */
DATA top(SIMPLE_STACK* s) {
    return s->stack[s->n_elems - 1];
}

/**
 * @brief Verifica o tipo do elemento no topo da stack.
 *
 * @param s Stack.
 * @returns Tipo do elemento no topo da stack.
 */
TYPE topType(STACK* s) {
    DATA elem = top(s->ss);
    return elem.type;
}

/**
 * @brief Encontra o elemento numa dada posição da stack.
 *
 * @param s Stack simples.
 * @param index Posição da stack.
 * @returns Elemento na posição dada.
 */
DATA getElemAtPosFromStack(SIMPLE_STACK* s, int index) {
    return (s->stack[(s->n_elems) - index - 1]);
}

/**
 * @brief Verifica se uma stack está vazia.
 *
 * @param s Stack.
 * @returns Inteiro.
 */
int is_empty(SIMPLE_STACK* s) {
    return s->n_elems == 0;
}

/**
 * @brief Cria uma stack.
 *
 * @returns s Stack.
 */
SIMPLE_STACK* create_array() {
    SIMPLE_STACK* s = alocMemSS;
    initStackArr(s, 10);
    return s;
}
/**
 * @brief Devolve o último Array aberto da stack.
 *
 * @param s Stack.
 * @return O último Array aberto da stack
 */
SIMPLE_STACK* getLastOpenArray(STACK* s) {
    return LastOpenArray;
}

/**
 * @brief Imprime os elementos da stack.
 *
 * @param s Stack.
 */
void print_stack(SIMPLE_STACK* s) {
    // putchar('\n');
    DATA elem = s->stack[0];
    if ((elem.type == CHAR && elem.value.CHAR != '\n') || elem.type != CHAR) {
        tryPrintLong(&elem);
    }
    for (int K = 1; K < s->n_elems; K++) {
        DATA elem = s->stack[K];
        tryPrintLong(&elem);
    }
}

/**
 * @brief Esta função imprime o valor de um DATA
 *
 * Imprime o valor de um DATA caso o seu tipo seja Long, caso contrário chama
 * outro caso
 *
 * @param e Um qualquer DATA.
 */
void tryPrintLong(DATA* e) {
    if (e->type == LONG)
        printf("%ld", e->value.LONG);
    else
        tryPrintDouble(e);
}

/**
 * @brief Esta função imprime o valor de um DATA
 *
 * Imprime o valor de um DATA caso o seu tipo seja Double, caso contrário chama
 * outro caso.
 *
 * @param e Um qualquer DATA.
 */
void tryPrintDouble(DATA* e) {
    if (e->type == DOUBLE)
        printf("%g", e->value.DOUBLE);
    else
        tryPrintChar(e);
}
/**
 * @brief Esta função imprime o valor de um DATA
 *
 * Imprime o valor de um DATA caso o seu tipo seja Char, caso contrário chama
 * outro caso.
 *
 * @param e Um qualquer DATA.
 */
void tryPrintChar(DATA* e) {
    if (e->type == CHAR)
        printf("%c", e->value.CHAR);
    else
        tryPrintArr(e);
}

/**
 * @brief Esta função imprime o valor de um DATA
 *
 * Imprime o valor de um DATA caso o seu tipo seja Array.
 *
 * @param e Um qualquer DATA.
 */
void tryPrintArr(DATA* e) {
    // putchar('[');
    if (e->type == ARRAY) print_stack(e->value.AP);
    tryPrintBlock(e);
    // putchar(']');
}

/**
 * @brief Esta função imprime o valor de um DATA
 *
 * Imprime o valor de um DATA caso o seu tipo seja Block.
 *
 * @param e Um qualquer DATA.
 */
void tryPrintBlock(DATA* e) {
    if (e->type == BLOCK) printf("{ %s}", e->value.BLOCK);
    // printf("\n[ ] profundidade: %d, n_elems: %d\n", AP->profundidade, AP->n_elems);
}

/**
 * @brief Obtém o valor numérico de um DATA.
 *
 * @param e Corresponde ao DATA.
 * @returns Valor numérico do DATA e.
 */
double getNumericValue(DATA const* e) {
    double r = 0;
    if (e->type == LONG) r = e->value.LONG;
    if (e->type == DOUBLE) r = e->value.DOUBLE;
    if (e->type == CHAR) r = e->value.CHAR;
    return r;
}

/**
 * @brief Obtém o valor inteiro de um DATA.
 *
 * @param e Corresponde ao DATA.
 * @returns Valor inteiro do DATA e.
 */
long getIntegerValue(DATA* e) {
    long r = 0;
    if (e->type == LONG) r = e->value.LONG;
    if (e->type == DOUBLE) r = e->value.DOUBLE;
    if (e->type == CHAR) r = e->value.CHAR;
    return r;
}
/**
 * @brief Obtém o valor de um Array de um dado.
 *
 * @param e Corresponde ao dado.
 * @returns O valor de um Array.
 */
SIMPLE_STACK* getArrValue(DATA* e) {
    return e->value.AP;
}
/**
 * @brief Obtém o tipo resultante para uma próxima operação.
 *
 * @param s Stack.
 * @returns O tipo resultante.
 */
TYPE getResultingTypeForNextOp(STACK* s) {
    SIMPLE_STACK* ss = s->ss;
    DATA e = ss->stack[ss->n_elems - 2];
    return getResultingType(top(ss), e);
}
/**
 * @brief Obtém o tipo resultante para uma operação.
 *
 * @param x Dado 1.
 * @param y Dado 2.
 * @returns O tipo resultante.
 */
TYPE getResultingType(DATA x, DATA y) {
    return (y.type > x.type) ? y.type : x.type;
}

/**
 * @brief Verifica o tipo de um dado.
 *
 * @param elem Corresponde ao dado a ser analisado.
 * @param mask Mask.
 * @returns Inteiro.
 */
int has_type(DATA elem, int mask) {
    return (elem.type & mask) != 0;
}

/**
 * @brief Inicializa um dado de um array.
 *
 * @param s Stack.
 * @returns O dado.
 */
DATA initArrElem(SIMPLE_STACK* s) {
    DATA e;
    e.type = ARRAY;
    e.value.AP = s;
    return e;
}
/**
 * @brief Duplica um dado.
 *
 * @param fancy Dado.
 * @returns O Dado clonado.
 */
DATA cloneDATA(DATA const* fancy) {
    DATA e;
    e.type = fancy->type;
    if (e.type == ARRAY) {
        e.value.AP = cloneArray(fancy->value.AP);
    } else if (e.type == BLOCK) {
        e.value.BLOCK = strdup(fancy->value.BLOCK); // change this
    } else {
        storeData(&e, e.type, getNumericValue(fancy));
    }
    return e;
}
/**
 * @brief Duplica um Array e coloca-o na stack.
 *
 * @param s Stack.
 * @returns O Array clonado.
 */
SIMPLE_STACK* cloneArray(SIMPLE_STACK const* s) {
    SIMPLE_STACK* ns = create_array();
    for (int i = 0; i < s->n_elems; i++) {
        pushss(ns, cloneDATA(&s->stack[i]));
    }
    return ns;
}
/**
 * @brief Devolve a stack ligada a uma stack.
 *
 * @param s Stack.
 * @returns A stack ligada.
 */
DATA* topPointer(STACK* s) {
    SIMPLE_STACK* lastArr = s->OpenArrayPtr->stack[s->OpenArrayPtr->n_elems - 1].value.AP;
    return &lastArr->stack[lastArr->n_elems - 1];
}
/**
 * @brief Devolve a stack ligada a uma stack.
 *
 * @param s Stack.
 * @returns A stack ligada.
 */
SIMPLE_STACK ssp(STACK* s) {
    return *s->ss;
}