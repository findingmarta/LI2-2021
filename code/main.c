/**
 * @file Este ficheiro contém a main do guiao 1.
 */

#include "inputoutput.h"
#include "parser.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Esta é a função principal do programa que irá ler a linha.
 *
 * O programa irá parar caso o espaço não seja suficiente. Invoca a função parser.
 * @returns O valor 0.
 */
int main() {
    char* line = readLine();
    STACK* s = create_stack();
    initVarsArr(s);
    parse(line, s);
    print_stack(s->ss);
    printf("\n");
    // freeStack(s);
    free(line);
    return 0;
}