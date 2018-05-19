//
// Created by pvcorazza on 5/18/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

AST *nodo_raiz;
void semantic_analisys (AST *node) {

	//guarda a raiz recebida
	nodo_raiz = node;
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

		case AST_CHAMADA_FUNCAO:
			if(node->symbol->type != SYMBOL_FUNCTION){
					fprintf(stderr, "ERRO: identificador deve ser uma funcao.\n");
					exit(4);
			}

			int qtd_param_chamada = 0;
			int qtd_param_def_funcao = node->symbol->qtd_parametros;

			// conta quantos parametros a a funcao recebeu na sua chamada
			qtd_param_chamada = conta_parametros(node->son[0]);

			if(qtd_param_chamada != qtd_param_def_funcao){
				fprintf(stderr, "[LINE %d] ERRO: Quantidade de parametros diferentes na chamada da funcao.\n",node->line_number);
				exit(4);
			}

			if(qtd_param_chamada != 0 ){

				//ta dando segmentation fault

				//verifica_tipos_parametros_funcao(node);
			}

			/*

			// Agora verifica se os tipos dos parametros estao corretos com a definicao:
			if(qtd_param_chamada !=0){

				//procura a declaracao da funcao
				AST * nodo_decl_funcao = procura_def_funcao(nodo_raiz,node->symbol->text);


				for(int parametro=0; parametro < qtd_param_def_funcao;parametro++){

					if(nodo_decl_funcao->son[0]->symbol->datatype != node->son[0]->symbol->datatype)
					{
						fprintf(stderr, "ERRO: Tipo de parametro incorreto\n");
						exit(4);
					}

					//percorre a lista de parametros (descendo a arvore)
					node = node->son[1];
					nodo_decl_funcao = nodo_decl_funcao->son[1];
				}

			}

			*/

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

AST *procura_def_funcao(AST *node, char *nome)
{
	int i;
	AST *funcao;

	if(!node) return 0;

	if(node->type == AST_DEF_FUNCAO)
		if(node->symbol != NULL){
			if(strcmp(node->symbol->text, nome) == 0)
				return node;
		}

	for(i=0; i < MAX_SONS; i++)
		if(node->son[i] != NULL) {
			funcao = procura_def_funcao(node->son[i], nome);
			if(funcao)
				return funcao;
		}
	return 0;
}

void verifica_tipos_parametros_funcao(AST* nodecall)
{
	if(!nodecall) return;
	AST* nodedef;
	if(nodecall->symbol != NULL) nodedef = procura_def_funcao(nodo_raiz, nodecall->symbol->text);

	if(nodecall->son[0] != NULL && nodedef->son[1] != NULL)
	{
		nodecall = nodecall->son[0];
		nodedef = nodedef->son[1];

		while(nodecall != NULL && nodedef != NULL)
		{
			if(nodecall->son[0]->symbol != NULL && nodedef->son[0]->symbol != NULL)
				{
					if(nodecall->son[0]->symbol->datatype > nodedef->son[0]->symbol->datatype)
					{
						fprintf(stderr, "[LINE %d] ERRO: Tipo de parametro incorreto\n",nodecall->line_number);
						exit(4);
					}
				}
				nodecall = nodecall->son[1];
				nodedef = nodedef->son[1];
		}
	}
}
