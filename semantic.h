//
// Created by pvcorazza on 5/18/18.
//

#ifndef TESTE_SEMANTIC_H
#define TESTE_SEMANTIC_H

#include "ast.h"

void semantic_analisys (AST *node);
void set_declarations(AST *node);
void check_undeclared (AST *node);
void check_usage(AST *node);
int conta_parametros(AST *node);
AST *procura_def_funcao(AST *node, char *nome);
void verifica_atribuicao_ponteiros(AST *node);
AST *procura_declaracao_ponteiro(AST *node, char *nome);
AST *procura_declaracao_global(AST *node, char *nome);
#endif //TESTE_SEMANTIC_H
