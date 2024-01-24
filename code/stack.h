/**
 * @file Este ficheiro contém as funções principais da stack.
 */

#ifndef STACK_H
#define STACK_H

/**
 * @brief Define tipos.
 */
typedef enum {
    EMPTY = 0,  ///< Tipo Empty(usado para quando a stack esta vazia)
    CHAR = 1,   ///< Tipo char
    LONG = 2,   ///< Tipo long
    DOUBLE = 4, ///< Tipo double
    ARRAY = 8,  ///< Tipo array
    BLOCK = 32  ///< Tipo block
} TYPE;

#define INTEGER (LONG | CHAR)     ///< Corresponde a um integer.
#define NUMBER (INTEGER | DOUBLE) ///< Corresponde a um número.

struct simple_stack;

/**
 * @brief Define dados.
 */
typedef struct {
    union {
        long LONG;               /**< Valor do tipo long*/
        double DOUBLE;           /**< Valor do tipo double*/
        char CHAR;               /*!< Valor do tipo char. */
        struct simple_stack* AP; /*!< Valor do tipo array. */
        char* BLOCK;             /*!< Valor do tipo bloco. */
    } value;                     /*!< Valor. */
    TYPE type; /*!< Variável que corresponde ao tipo do dado (long, double, char ou string). */
} DATA;

/**
 * @brief Define uma stack.
 */
typedef struct simple_stack {
    DATA* stack; /*!< Corresponde aos elementos da stack. */
    int size;    /*!< Um valor inteiro que irá corresponder ao tamanho da stack. */
    int n_elems; /*!< Um valor inteiro que irá corresponder ao topo da stack. */
} SIMPLE_STACK;

/**
 * @brief Define uma stack.
 */
typedef struct stack {
    SIMPLE_STACK* ss; /*!< Corresponde a uma stack propriamente dita */
    DATA* vars; /*!< Corresponde às variáveis da stack. */
    SIMPLE_STACK* OpenArrayPtr;/*!< Para manipular Arrays. */
    char hasOpenStr; /*!< Permite-nos saber se existe uma string aberta. */
    char hasOpenBlock; /*!< Permite-nos saber se existe um bloco aberto. */
} STACK;

/**
 * @brief Cria uma stack.
 *
 * @param size Tamanho da stack.
 * @returns s Stack.
 */
STACK* create_stack();
void initStackSubstacks(STACK* s);
void initStackArr(SIMPLE_STACK* s, int size);
void initVarsArr(STACK* s);
void initVarsArrP1(STACK* s);
void initVarsArrP2(STACK* s);
void initVarsArrP3(STACK* s);

void freeStack(STACK* s);
void freeData(DATA e);
void freeSimpleStack(SIMPLE_STACK* ss);
void freeAllArraysInStack(SIMPLE_STACK* s);

void push(STACK* s, DATA elem);
void pushss(SIMPLE_STACK* s, DATA elem);
DATA pop(STACK* s);
DATA popss(SIMPLE_STACK* s);
void pushNumValue(STACK* s, TYPE t, double v);
void pushArrValue(STACK* s, SIMPLE_STACK* ss);

void needsMoreMem(SIMPLE_STACK* s);
void needsLessMem(SIMPLE_STACK* s);

DATA top(SIMPLE_STACK* s);
TYPE topType(STACK* s);
DATA getElemAtPosFromStack(SIMPLE_STACK* s, int index);
int is_empty(SIMPLE_STACK* s);

SIMPLE_STACK* create_array();
SIMPLE_STACK* getLastOpenArray(STACK* s);
/**
 * @brief Imprime os elementos da stack.
 *
 * @param s Stack.
 */
void print_stack(SIMPLE_STACK* s);
void tryPrintLong(DATA* e);
void tryPrintDouble(DATA* e);
void tryPrintChar(DATA* e);
void tryPrintArr(DATA* e);
void tryPrintBlock(DATA* e);

/**
 * @brief Verifica o tipo de um dado.
 *
 * @param elem Corresponde ao dado a ser analisado.
 * @returns Inteiro
 */
int has_type(DATA elem, int mask);

DATA initArrElem(SIMPLE_STACK* s);

void storeData(DATA* e, TYPE t, double v);
void tryStoreDataD(DATA* e, TYPE t, double v);
void tryStoreDataL(DATA* e, TYPE t, double v);
void tryStoreDataC(DATA* e, TYPE t, double v);

double getNumericValue(DATA const* e);
long getIntegerValue(DATA* e);
SIMPLE_STACK* getArrValue(DATA* e);
TYPE getResultingTypeForNextOp(STACK* s);
TYPE getResultingType(DATA x, DATA y);

DATA cloneDATA(DATA const* fancy);
SIMPLE_STACK* cloneArray(SIMPLE_STACK const* s);

DATA* topPointer(STACK* s);

SIMPLE_STACK ssp(STACK* s);

#define LastOpenArray top(s->OpenArrayPtr).value.AP ///< Define LastOpenArray como o Array no topo.

#endif