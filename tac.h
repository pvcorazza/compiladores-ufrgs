//
// Created by pvcorazza on 5/25/18.
//

#ifndef ETAPA5_TAC_H
#define ETAPA5_TAC_H

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5



#define TAC_L 6
#define TAC_G 7
#define TAC_LE 8
#define TAC_GE 9
#define TAC_EQ 10
#define TAC_NE 11
#define TAC_AND 12
#define TAC_OR 13
#define TAC_NOT 14
//#define TAC_MOVE 15
#define TAC_READ 16
//#define TAC_RET 17
#define TAC_PRINT 18
#define TAC_CALL 19
#define TAC_IFZ 20
#define TAC_LABEL 21
#define TAC_JUMP 22
#define TAC_BEGINFUN 23
#define TAC_ENDFUN 24
#define TAC_VECREAD 25
#define TAC_VECWRITE 26
//#define TAC_AINIPUSH 27
//#define TAC_ASIZE 28
//#define TAC_PARPOP 29
//#define TAC_PARPUSH 30

#define TAC_ASS 50

#include "ast.h"


typedef struct tac{
    int type;
    hash_entry *res;
    hash_entry *op1;
    hash_entry *op2;
    struct tac *prev;
    struct tac *next;
} TAC;

TAC* tac_create(int type, hash_entry *res, hash_entry *op1, hash_entry *op2);
void tac_print_single(TAC *tac);
void tac_print_back(TAC *tac);
TAC* tac_reverse(TAC* last);
void tac_print_forward(TAC* tac);
TAC* tac_join(TAC* l1, TAC* l2);
TAC* code_generator(AST *node);
TAC* make_bin_op (int type, TAC* code0, TAC* code1);
TAC* make_if_then(TAC* code0, TAC* code1);
TAC* make_if_then_else(TAC* code0, TAC* code1, TAC* code2);
TAC* make_while(TAC* code0, TAC* code1);
TAC* make_fun_def(AST* node, TAC* code0, TAC* code1, TAC* code2);

#endif //ETAPA5_TAC_H
