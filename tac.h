//
// Created by pvcorazza on 5/25/18.
//

#ifndef ETAPA4_TAC_H
#define ETAPA4_TAC_H

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5

#include "symbol_table.h"


typedef struct tac{
    int type;
    hash_entry *res;
    hash_entry *op1;
    hash_entry *op2;
    struct tac *prev;
    struct tac *next;
} TAC;

TAC* tac_create(int type, hash_entry *res, hash_entry *op1, hash_entry *op2);


#endif //ETAPA4_TAC_H
