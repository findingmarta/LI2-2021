

#include "stack.h"

void isBlockOp(STACK* s, char* op);

int isBlockOpCheck(STACK* s, char* op);

void blockOp(STACK* s, char op);

int tryOpenBlock(STACK* s, char op);

void blockExecuteOp(STACK* s, char op);

void executeBlockOnceOp(STACK* s);
STACK* executeBlockSimple(STACK* s, STACK* st, char* block, DATA e);
STACK* executeBlockFoldable(STACK* s, STACK* st, char* block, DATA e, DATA total);
void pushAllElemsToExistingStack(STACK* src, STACK* dst);

void applyBlockToArrayOp(STACK* s);

void filterArrayByBlockOp(STACK* s);

void foldArrayByBlockOp1(STACK* s);

void stopBlockExecIfFalse(STACK* s);

void sortOnBlockOp(STACK* s);
void insertInTheOrderedStack(STACK* s);

/**
 * @brief Define dados.
 */
typedef struct sortHelper {
    DATA trueValue; /**< Valor a ser inserido na stack.*/
    DATA procValue; /**< Valor a ser procurado*/
} sortHelper;       /**< Irá ajudar na implementação de blocos de ordenação.*/

int blockBuilder(STACK* s, char* token, int* size);