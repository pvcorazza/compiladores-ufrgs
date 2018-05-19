//
// Created by pvcorazza on 5/18/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"

void semantic_analisys (AST *node) {

    set_declarations(node);
    check_undeclared(node);
	check_usage(node);
}


void set_declarations(AST *node) {
    int i;
    if (!node) return;

    if (node->type == AST_DECL_GLOBAL || node->type == AST_VETOR_VAZIO ||
		node->type == AST_VETOR_INIC || node->type == AST_DECL_PONTEIRO || node->type == AST_DEF_FUNCAO) {
        if (node->symbol->type != SYMBOL_IDENTIFIER) {
            fprintf(stderr, "[LINE %d] Semantic Error: Symbol '%s' already declared.\n",  node->line_number, node->symbol->text);
            exit(4);
        } else {
            switch (node->type) {
                case AST_DECL_GLOBAL: node->symbol->type = SYMBOL_SCALAR;
                    break;
                case AST_VETOR_VAZIO: node->symbol->type = SYMBOL_VECTOR;
                    break;
                case AST_VETOR_INIC: node->symbol->type = SYMBOL_VECTOR;
                    break;
                case AST_DECL_PONTEIRO: node->symbol->type = SYMBOL_POINTER;
                    break;
                case AST_DEF_FUNCAO:
					node->symbol->type = SYMBOL_FUNCTION;
					node->symbol->qtd_parametros = conta_parametros(node->son[1]);
                    break;
                default: break;
            }
            if (node->son[0]->type == AST_INT) node->symbol->datatype = DATATYPE_INT;
            if (node->son[0]->type == AST_FLOAT) node->symbol->datatype = DATATYPE_FLOAT;
            if (node->son[0]->type == AST_CHAR) node->symbol->datatype = DATATYPE_CHAR;
        }
    }

	//Verifica declaracao dos parametros da funcao
	if(node->type == AST_PARAM){
		if (node->symbol->type != SYMBOL_IDENTIFIER) {
            fprintf(stderr, "[LINE %d] Semantic Error: Symbol '%s' already declared.\n",  node->line_number, node->symbol->text);
            exit(4);
        }
		else{

			node->symbol->type = SYMBOL_SCALAR;

			if (node->son[0]->type == AST_INT) node->symbol->datatype = DATATYPE_INT;
            if (node->son[0]->type == AST_FLOAT) node->symbol->datatype = DATATYPE_FLOAT;
            if (node->son[0]->type == AST_CHAR) node->symbol->datatype = DATATYPE_CHAR;
		}
	}

    for (i = 0; i < MAX_SONS; ++i) {
        set_declarations(node->son[i]);
    }
}

void check_undeclared(AST *node){
    if(!node) return;
    for (int i=0; i<MAX_SONS; i++) {
        check_undeclared(node->son[i]);
    }

    if(node->symbol != 0 && node->symbol->type == SYMBOL_IDENTIFIER){
        fprintf(stderr, "[LINE %d] Semantic Error: Symbol '%s' is not declared.\n",  node->line_number, node->symbol->text);
        exit(4);
    }
}

void check_usage(AST *node){
	int i;
    if (!node) return;

	switch(node->type){
		case AST_ATRIBUICAO:
			if(node->symbol->type != SYMBOL_SCALAR)
				{
					fprintf(stderr, "ERRO: identificador deve ser escalar.\n");
					exit(4);
				}

			break;

	}

	for(i=0; i <MAX_SONS; ++i)
		check_usage(node->son[i]);
}

int conta_parametros(AST *node){
	if(!node) return 0;
	else{
		return 1 + conta_parametros(node->son[1]);
	}
	return 0;
}
