//
// Created by pvcorazza on 5/25/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "tac.h"


TAC* make_fun_def(AST* node, TAC* code0, TAC* code1, TAC* code2)
;

//Cria uma TAC no formato TAC(TIPO,RESULTADO,OP1,OP2)
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

void tac_print_single(TAC *tac)
{
    if (!tac) return;
    if(tac->type == TAC_SYMBOL) return;
    fprintf(stderr, "TAC(");
    switch(tac->type){
        case TAC_SYMBOL: fprintf(stderr, "TAC_SYMBOL"); break;
        case TAC_ADD: fprintf(stderr, "TAC_ADD"); break;
        case TAC_SUB: fprintf(stderr, "TAC_SUB"); break;
        case TAC_MUL: fprintf(stderr, "TAC_MUL"); break;
        case TAC_DIV: fprintf(stderr, "TAC_DIV"); break;
        case TAC_L: fprintf(stderr, "TAC_L"); break;
        case TAC_G: fprintf(stderr, "TAC_G"); break;
        case TAC_LE: fprintf(stderr, "TAC_LE"); break;
        case TAC_GE: fprintf(stderr, "TAC_GE"); break;
        case TAC_EQ: fprintf(stderr, "TAC_EQ"); break;
        case TAC_NE: fprintf(stderr, "TAC_NE"); break;
        case TAC_AND: fprintf(stderr, "TAC_AND"); break;
        case TAC_OR: fprintf(stderr, "TAC_OR"); break;
        case TAC_NOT: fprintf(stderr, "TAC_NOT"); break;
//        case TAC_MOVE: fprintf(stderr, "TAC_MOVE"); break;
        case TAC_READ: fprintf(stderr, "TAC_READ"); break;
//        case TAC_RET: fprintf(stderr, "TAC_RET"); break;
        case TAC_IFZ: fprintf(stderr, "TAC_IFZ"); break;
        case TAC_LABEL: fprintf(stderr, "TAC_LABEL"); break;
        case TAC_JUMP: fprintf(stderr, "TAC_JUMP"); break;
        case TAC_PRINT: fprintf(stderr, "TAC_PRINT"); break;
//        case TAC_PARPOP: fprintf(stderr, "TAC_PARPOP"); break;
//        case TAC_FUNCALL: fprintf(stderr, "TAC_FUNCALL"); break;
        case TAC_BEGINFUN: fprintf(stderr, "TAC_BEGINFUN"); break;
        case TAC_ENDFUN: fprintf(stderr, "TAC_ENDFUN"); break;
        case TAC_VECREAD: fprintf(stderr, "TAC_VECREAD"); break;
        case TAC_VECWRITE: fprintf(stderr, "TAC_VECWRITE"); break;
//        case TAC_AINIPUSH: fprintf(stderr, "TAC_AINIPUSH"); break;
//        case TAC_ASIZE: fprintf(stderr, "TAC_ASIZE"); break;
//        case TAC_PARPUSH: fprintf(stderr, "TAC_PARPUSH"); break;

        case TAC_ASS: fprintf(stderr, "TAC_ASS"); break;

        default: fprintf(stderr, "TAC_UNKNOWN"); break;
    }
    if(tac->res) {
        fprintf(stderr, ",%s", tac->res->text);
    } else {
        fprintf(stderr, ",0");
    }
    if(tac->op1) {
        fprintf(stderr, ",%s", tac->op1->text);
    } else {
        fprintf(stderr, ",0");
    }
    if(tac->op2) {
        fprintf(stderr, ",%s", tac->op2->text);
    } else {
        fprintf(stderr, ",0");
    }
    fprintf(stderr, ")\n");
}

void tac_print_back(TAC *tac)
{


    if (!tac) return;

//    fprintf(stderr, "\n ----Impressão do código gerado----\n");
    tac_print_single(tac);
    tac_print_back(tac->prev);

//    TAC *tac;
//
//    for(tac = last; tac; tac = tac->prev){
//        tacPrintOne(tac);
//    }
}

TAC* tac_reverse(TAC* last)
{
    TAC* tac = 0;
    for(tac = last; tac->prev; tac = tac->prev)
        tac->prev->next = tac;
    return tac;
}

void tac_print_forward(TAC* tac)
{
    if(!tac) return;

    tac_print_single(tac);
    tac_print_forward(tac->next);

}

//Concatena TACs l1 e l2.
TAC* tac_join(TAC* l1, TAC* l2)
{
    TAC* aux = 0;
    if(!l1) return l2;
    if(!l2) return l1;

    for (aux = l2; aux->prev; aux = aux->prev);

    aux->prev = l1;

    return l2;
}


TAC* code_generator(AST *node)
{
    int i;
    TAC *code[MAX_SONS];
    TAC *result;

    if(!node) return 0;

    //Gera código para todos os filhos do nodo atual, os códigos gerados estarão em code[i], onde i é a posição do filho.
    for(i = 0; i < MAX_SONS; ++i)
    {
        code[i] = code_generator(node->son[i]);
    }

    switch(node->type)
    {

        case AST_SYMBOL: result = tac_create(TAC_SYMBOL, node->symbol, 0, 0); break;
        case AST_SOMA: result = make_bin_op(TAC_ADD,code[0],code[1]); break;
        case AST_ATRIBUICAO: result = tac_join(code[0],tac_create(TAC_ASS, node->symbol, code[0]?code[0]->res:0, 0)); break;
        case AST_SUB: result = make_bin_op(TAC_SUB,code[0],code[1]); break;
        case AST_MUL: result = make_bin_op(TAC_MUL,code[0],code[1]); break;
        case AST_DIV: result = make_bin_op(TAC_DIV,code[0],code[1]); break;
        case AST_L: result = make_bin_op(TAC_L,code[0],code[1]); break;
        case AST_G: result = make_bin_op(TAC_G,code[0],code[1]); break;
        case AST_LE: result = make_bin_op(TAC_LE,code[0],code[1]); break;
        case AST_GE: result = make_bin_op(TAC_GE,code[0],code[1]); break;
        case AST_EQ: result = make_bin_op(TAC_EQ,code[0],code[1]); break;
        case AST_NE: result = make_bin_op(TAC_NE,code[0],code[1]); break;
        case AST_AND: result = make_bin_op(TAC_AND,code[0],code[1]); break;
        case AST_OR: result = make_bin_op(TAC_OR,code[0],code[1]); break;
        case AST_NOT: result = tac_join(code[0],tac_create(TAC_NOT,make_temp(),code[0]?code[0]->res:0,0)); break;
        case AST_READ: result = tac_create(TAC_READ, node->symbol, 0, 0); break;
//        case ASTREE_RETURN: result = tacJoin(code[0], tacCreate(TAC_RET, node->symbol, code[0]?code[0]->res:0, 0)); break;
        case AST_IF: result = make_if_then(code[0], code[1]); break;
        case AST_IF_ELSE: result = make_if_then_else(code[0], code[1], code[2]); break;
        case AST_WHILE: result = make_while(code[0], code[1]); break;
        case AST_LISTA_PRINT: result = tac_join(code[0],tac_join(code[1],tac_create(TAC_PRINT, code[1]?code[1]->res:0, 0, 0))); break;

//        case ASTREE_FUNCALL: result = tacJoin(code[0], tacCreate(TAC_FUNCALL, makeTemp(), node->symbol, 0)); break;
//        case ASTREE_PARCALLL: result = tacJoin(code[1], tacCreate(TAC_PARPUSH, code[0]?code[0]->res:0, 0, 0)); break;
        case AST_DEF_FUNCAO: result = make_fun_def(node, code[0], code[1], code[2]); break;
//        case ASTREE_PARAM: result = tacCreate(TAC_PARPOP, node->symbol, 0, 0); break;
//        case ASTREE_PARL: result = tacJoin(code[0], code[1]); break;
//
//
        case AST_VET: result = tac_join(code[0],tac_create(TAC_VECREAD, make_temp(), node->symbol, code[0]?code[0]->res:0)); break;
        case AST_ATRIBUICAO_VETOR: result = tac_join(code[0], tac_join(code[1], tac_create(TAC_VECWRITE, node->symbol, code[0]?code[0]->res:0, code[1]?code[1]->res:0))); break;
//        case ASTREE_INTL: result = tacJoin(code[0], tacCreate(TAC_AINIPUSH, node->symbol, 0, 0)); break;

        default: result = tac_join(tac_join(tac_join(code[0], code[1]), code[2]), code[3]) ; break;

    }

    return result;
}

//Resultado de ambos os filhos do nodo atual são os operadores do TAC, resultado da operação vai para um temporário.
TAC* make_bin_op (int type, TAC* code0, TAC* code1) {
    TAC* new_tac = tac_create(type, make_temp(),code0?code0->res:0,code1?code1->res:0);
    return tac_join(code0, tac_join(code1, new_tac));
}

// if (code0) then (code1)
TAC* make_if_then(TAC* code0, TAC* code1)
{
    TAC* if_TAC = 0;
    TAC* false_label_TAC = 0;
    hash_entry* false_label = 0;

    false_label = make_label();
    if_TAC = tac_create(TAC_IFZ, false_label, code0?code0->res:0, 0);

    false_label_TAC = tac_create(TAC_LABEL, false_label, 0, 0);

    return tac_join(tac_join(tac_join(code0, if_TAC), code1),false_label_TAC);
}

// if (code0) then (code1) else (code2)
TAC* make_if_then_else(TAC *code0, TAC *code1, TAC *code2)
{

    TAC* if_tac = 0;
    TAC* false_label_TAC = 0;
    TAC* jump_label_TAC = 0;

    hash_entry* false_label = 0;
    hash_entry* jump_label = 0;

    false_label = make_label();
    jump_label = make_label();

    if_tac = tac_create(TAC_IFZ, false_label, code0?code0->res:0, 0);
    false_label_TAC = tac_create(TAC_LABEL, false_label, 0, 0);
    jump_label_TAC = tac_create(TAC_LABEL, jump_label, 0, 0);

    TAC* jump_TAC = tac_create(TAC_JUMP, jump_label, 0, 0);

    return tac_join(tac_join(tac_join(tac_join(tac_join(tac_join(code0, if_tac),code1),jump_TAC),false_label_TAC),code2),jump_label_TAC);
}

// while (code0) (code1)
TAC* make_while(TAC* code0, TAC* code1)
{
    TAC* while_TAC;
    TAC* false_label_TAC;
    TAC* jump_label_TAC;

    hash_entry *jump_label = make_label();
    hash_entry *false_label = make_label();

    TAC* jump_TAC = tac_create(TAC_JUMP, jump_label, 0, 0);
    while_TAC = tac_create(TAC_IFZ, false_label, code0?code0->res:0, 0);

    jump_label_TAC = tac_create(TAC_LABEL, jump_label, 0, 0);
    false_label_TAC = tac_create(TAC_LABEL, false_label, 0, 0);

    return tac_join(tac_join(tac_join(tac_join(tac_join(jump_label_TAC, code0), while_TAC), code1),jump_TAC), false_label_TAC);
}


// begin_fun (code1) (code2)
TAC* make_fun_def(AST* node, TAC* code0, TAC* code1, TAC* code2)
{
    TAC* tac_beg_fun = tac_create(TAC_BEGINFUN, node->symbol, 0, 0);
    TAC* tac_end_fun = tac_create(TAC_ENDFUN, node->symbol, 0, 0);

    return tac_join(tac_beg_fun, tac_join(code1, tac_join(code2, tac_end_fun)));
}