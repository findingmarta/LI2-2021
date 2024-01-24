

#include "blocks.h"

#include "arrays.h"
#include "logics.h"
#include "manipulations.h"
#include "parser.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Verifica se o operador é um operador de bloco.
 *
 * Caso não se trate de um operador de bloco, passa à decisão seguinte.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void isBlockOp(STACK* s, char* op) {
    if (isBlockOpCheck(s, op))
        blockOp(s, op[0]);
    else
        isArrayOp(s, op);
}

/**
 * @brief Funcão auxiliar para verificar se o operador é um operador de bloco.
 *
 * Avalia se o bloco é válido.
 *
 * @param s Stack.
 * @param op Apontador para o símbolo do operador.
 * @returns Valor lógico.
 */
// l { ( ( } %
int isBlockOpCheck(STACK* s, char* op) {
    SIMPLE_STACK* lastArr = getLastOpenArray(s);
    int isOperator1 = (strstr(" ~%*,$w", op) && 1);
    int isLastElemBlock = (top(lastArr).type == BLOCK);
    int isElemBlock = lastArr->stack[lastArr->n_elems - 2].type == BLOCK || isLastElemBlock;
    int isOperator2 = op[0] == '{';
    return (isOperator1 && isElemBlock) || isOperator2;
}

/**
 * @brief Funcão auxiliar para verificar se é possível executar um bloco.
 *
 * O bloco será executado caso seja possível criar um bloco.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void blockOp(STACK* s, char op) {
    if (!tryOpenBlock(s, op)) {
        blockExecuteOp(s, op);
    }
}

/**
 * @brief Função que verifica se é possível criar um bloco.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 * @returns valor lógico.
 */
int tryOpenBlock(STACK* s, char op) {
    if (op == '{') {
        s->hasOpenBlock = 1;
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Procedimento que trata de executar a operação de bloco de acordo com o operador.
 *
 * @param s Stack.
 * @param op Símbolo do operador.
 */
void blockExecuteOp(STACK* s, char op) {
    switch (op) {
        case '~':
            executeBlockOnceOp(s);
            break;
        case '%':
            applyBlockToArrayOp(s);
            break;
        case '*':
            foldArrayByBlockOp1(s);
            break;
        case ',':
            filterArrayByBlockOp(s);
            break;
        case '$':
            sortOnBlockOp(s);
            break;
        case 'w':
            stopBlockExecIfFalse(s);
            break;
        default:
            break;
    }
}

/**
 * @brief Procedimento responsável pela execução de um bloco.
 *
 * @param s Stack.
 */
void executeBlockOnceOp(STACK* s) {
    char* block = pop(s).value.BLOCK;
    DATA e = pop(s);
    STACK* st = create_stack();
    st = executeBlockSimple(s, st, block, e);
    pushAllElemsToExistingStack(st, s);
}

/**
 * @brief Função auxiliar que trata de executar um bloco.
 *
 * @param s Stack original.
 * @param st Stack atualizada.
 * @param block Bloco a ser executado.
 * @param e Dado retirado da stack s.
 * @returns Stack atualizada.
 */
STACK* executeBlockSimple(STACK* s, STACK* st, char* block, DATA e) {
    st->vars = s->vars;
    push(st, e);
    st = parse(block, st);
    s->vars = st->vars;
    return st;
}

/**
 * @brief Procedimento que junta os elementos das duas stacks.
 *
 * @param src Stack origem.
 * @param dst Stack destino.
 */
void pushAllElemsToExistingStack(STACK* src, STACK* dst) {
    for (int i = 0; i < src->ss->n_elems; i++) {
        push(dst, src->ss->stack[i]);
    }
}

/**
 * @brief Procedimento responsável por limpar uma stack.
 *
 * @param s Stack.
 */
void cleanStack(STACK* s) {
    while (s->ss->n_elems) {
        s->ss->stack[s->ss->n_elems - 1].type = EMPTY;
        s->ss->stack[s->ss->n_elems - 1].value.LONG = 0;
        s->ss->n_elems--;
    }
}

/**
 * @brief Procedimento responsável por aplicar um bloco a um array.
 *
 * @param s Stack.
 */
void applyBlockToArrayOp(STACK* s) {
    char* block = pop(s).value.BLOCK;
    SIMPLE_STACK* ss = pop(s).value.AP;
    STACK* st = create_stack();
    createArray(s);
    for (int i = 0; i < ss->n_elems; i++) {
        DATA e = ss->stack[i];
        st = executeBlockSimple(s, st, block, e);
        pushAllElemsToExistingStack(st, s);
        cleanStack(st);
    }
    closeArray(s);
}

/**
 * @brief Procedimento responsável por filtar um array utilizando um bloco.
 *
 * @param s Stack.
 */
void filterArrayByBlockOp(STACK* s) {
    char* block = pop(s).value.BLOCK;
    SIMPLE_STACK* ss = pop(s).value.AP;
    STACK* st = create_stack();
    createArray(s);
    for (int i = 0; i < ss->n_elems; i++) {
        DATA e = ss->stack[i];
        st = executeBlockSimple(s, st, block, e);
        if (!isElemFalse(pop(st))) {
            push(s, e);
        }
        cleanStack(st);
    }
    closeArray(s);
}

/**
 * @brief Função auxiliar que irá aplicar o fold.
 *
 * @param s Stack.
 * @param st Stack.
 * @param block Bloco a ser aplicado.
 * @param e Dado.
 * @param total Dado.
 * @returns Stack atualizada.
 */
STACK* executeBlockFoldable(STACK* s, STACK* st, char* block, DATA e, DATA total) {
    st->vars = s->vars;
    push(st, total);
    push(st, e);
    st = parse(block, st);
    s->vars = st->vars;
    return st;
}

/**
 * @brief Procedimento responsável por aplicar um fold sobre um array utilizando um bloco.
 *
 * @param s Stack.
 */
void foldArrayByBlockOp(STACK* s) {
    char* block = pop(s).value.BLOCK;
    SIMPLE_STACK* ss = pop(s).value.AP;
    DATA total = ss->stack[0];
    STACK* st = create_stack();
    cleanStack(st);
    for (int i = 1; i < ss->n_elems; i++) {
        DATA e = ss->stack[i];
        st = executeBlockFoldable(s, st, block, e, total);
        total = pop(st);
        cleanStack(st);
    }
    createArray(s);
    push(s, total);
    closeArray(s);
}
/**
 * @brief Função Auxiliar da responsável por aplicar fold sobre um array utilizando um bloco.
 *
 * @param src Stack origem
 * @param dest Stack destino
 * @param magia Número de elementos.
 */
void foldAux(STACK* src, SIMPLE_STACK* dest, int magia) {
    DATA tempData;
    int ne = src->ss->n_elems;
    for (int i = ne - magia; i < ne; i++) {
        tempData = cloneDATA(&src->ss->stack[i]);
        pushss(dest, tempData);
    }
    while (magia) {
        pop(src);
        magia--;
    }
}

/**
 * @brief Procedimento responsável por aplicar um fold sobre um array utilizando um bloco.
 *
 * @param s Stack.
 */
void foldArrayByBlockOp1(STACK* s) {
    char* block = pop(s).value.BLOCK;
    SIMPLE_STACK* ss = pop(s).value.AP;
    STACK* st = create_stack();
    cleanStack(st);
    int n = ss->n_elems, magia;
    DATA tempData;
    for (int i = 0; i < n; i++) {
        tempData = popss(ss);
        push(st, tempData);
    }
    st->vars = s->vars;
    n = st->ss->n_elems;
    createArray(s);
    for (int i = 0; i < n - 1; i++) {
        manipulationsOpTwo(st);
        st = parse(block, st);
        magia = st->ss->n_elems - (n - (i + 1)); // its a kind of magic
        foldAux(st, ss, magia);
    }
    // n = st->ss->n_elems;
    tempData = pop(st);
    push(s, tempData);
    pushArrValue(s, ss);
    concatOps(s);
    /* for (int i = 0; i < n; i++) {
        tempData = popss(ss);
        push(s, tempData);
    } */
    closeArray(s);
    s->vars = st->vars;
}

/**
 * @brief Procedimento responsável por parar a execução de um bloco.
 *
 * O bloco terminará a execução quanto o valor for falso ou um DATA vazio.
 *
 * @param s Stack.
 */
void stopBlockExecIfFalse(STACK* s) {
    char* block = pop(s).value.BLOCK;
    int check;
    DATA e = pop(s);
    do {
        push(s, e);
        parse(block, s);
        e = pop(s);
        check = !isElemFalse(e);
    } while (check);
    push(s, e);
}

/**
 * @brief Procedimento responsável por ordenar utilizando um bloco.
 *
 * @param s Stack.
 */
void sortOnBlockOp(STACK* s) {
    DATA block = pop(s);
    manipulationsOpOne(s, '_');
    push(s, block);
    applyBlockToArrayOp(s);
    insertInTheOrderedStack(s);
}
/**
 * @brief Subtrai dois valores de sortHelper.
 *
 * @param v1 Operando 1.
 * @param v2 Operando 2.
 *
 * @return Subtração de v1 por v2
 */
int comparitor(sortHelper v1, sortHelper v2) {
    return (getNumericValue(&v1.procValue) - getNumericValue(&v2.procValue));
}

/**
 * @brief Procedimento auxiliar  de ordenação.
 *
 * @param s Stack.
 */
void insertInTheOrderedStack(STACK* s) {
    SIMPLE_STACK *procArray = pop(s).value.AP, *initArray = pop(s).value.AP;
    int size = procArray->n_elems;
    sortHelper* arr = malloc(sizeof(sortHelper) * size);
    for (int i = 0; i < procArray->n_elems; i++) {
        arr[i].procValue = procArray->stack[i];
        arr[i].trueValue = initArray->stack[i];
    }

    for (int step = 1; step < size; step++) {
        sortHelper key = arr[step];
        int key1 = getNumericValue(&key.procValue);
        int j = step - 1;

        // Compare key with each element on the left of it until an element smaller than
        // it is found.
        // For descending order, change key<array[j] to key>array[j].
        while (key1 < getNumericValue(&arr[j].procValue) && j >= 0) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
    createArray(s);
    for (int i = 0; i < size; i++) {
        push(s, arr[i].trueValue);
    }

    closeArray(s);
    free(arr);
}

/**
 * @brief Função que trata de gerar um bloco.
 *
 * Irá alocar espaço para o novo bloco.
 *
 * @param s Stack.
 * @param token String token.
 * @param size Tamanho da String.
 * @returns O valor 1 ou 0.
 */
int blockBuilder(STACK* s, char* token, int* size) {
    if (s->hasOpenBlock == 1) {
        char* ntoken = malloc(sizeof(char) * strlen(token));
        for (size_t i = 0; i < strlen(token); i++) {
            ntoken[i] = token[i];
        }
        DATA ne;
        ne.value.BLOCK = ntoken;
        ne.type = BLOCK;
        push(s, ne);
        s->hasOpenBlock--;
        cleanStr(token, size);
        return 1;
    } else {
        return 0;
    }
}