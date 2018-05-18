//
// Created by pvcorazza on 5/18/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"

void set_declarations(AST *node) {
    int i;
    if (!node) return;
    if (node->type == AST_DECL_GLOBAL) {
        if (node->symbol->type != SYMBOL_IDENTIFIER) {
            fprintf(stderr, "Semantic error: Symbol '%s' already declared. Line \n", node->symbol->text);
            exit(4);
        } else {
            node->symbol->type = SYMBOL_SCALAR;
            if (node->son[0]->type == AST_INT) node->symbol->datatype = DATATYPE_INT;
//            if(node->son[0]->type == ASTREE_SHORT) node->symbol->datatype = SYMBOL_DATATYPE_SHORT;
//            if(node->son[0]->type == ASTREE_LONG) node->symbol->datatype = SYMBOL_DATATYPE_LONG;
//            if(node->son[0]->type == ASTREE_FLOAT) node->symbol->datatype = SYMBOL_DATATYPE_FLOAT;
//            if(node->son[0]->type == ASTREE_DOUBLE) node->symbol->datatype = SYMBOL_DATATYPE_DOUBLE;

        }
    }
    for (i = 0; i < MAX_SONS; ++i) {
        set_declarations(node->son[i]);
    }
}