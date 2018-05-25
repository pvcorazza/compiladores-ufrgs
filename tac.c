//
// Created by pvcorazza on 5/25/18.
//

#include <stdlib.h>
#include "tac.h"

TAC* tac_create(int type, hash_entry *res, hash_entry *op1, hash_entry *op2)
{
    TAC *new_tac = (TAC*) calloc(1, sizeof(TAC));
    new_tac->type = type;
    new_tac->res = res;
    new_tac->op1 = op1;
    new_tac->op2 = op2;
    new_tac->prev = 0;
    new_tac->next = 0;
    return new_tac;
}