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
    check_operands(node);
	check_usage(node);
}


void set_declarations(AST *node) {
    int i;
    if (!node) return;

    if (node->type == AST_DECL_GLOBAL || node->type == AST_VETOR_VAZIO ||
		node->type == AST_VETOR_INIC || node->type == AST_DECL_PONTEIRO ||
		node->type == AST_DEF_FUNCAO) {
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
					printf("%s\n", "Declaracao de ponteiro");
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

void check_operands(AST *node) {
    int i;
    if (!node) return;

    if (node->type == AST_SOMA || node->type == AST_SUB || node->type == AST_MUL || node->type == AST_DIV ||
        node->type == AST_L || node->type == AST_G || node->type == AST_LE || node->type == AST_GE || node->type == AST_EQ ||
        node->type == AST_NE || node->type == AST_AND || node->type == AST_OR) {
        if (node->son[0]->type == AST_L ||
            node->son[0]->type == AST_G ||
            node->son[0]->type == AST_LE ||
            node->son[0]->type == AST_GE ||
            node->son[0]->type == AST_EQ ||
            node->son[0]->type == AST_NE ||
            node->son[0]->type == AST_OR ||
            node->son[0]->type == AST_AND ||
            node->son[0]->type == AST_NOT) {

            fprintf(stderr, "[LINE %d] Semantic Error: Left operand can not be logical.\n", node->line_number);
            exit(4);
        }
        if (node->son[1]->type == AST_L   ||
            node->son[1]->type == AST_G   ||
            node->son[1]->type == AST_LE  ||
            node->son[1]->type == AST_GE  ||
            node->son[1]->type == AST_EQ  ||
            node->son[1]->type == AST_NE  ||
            node->son[1]->type == AST_OR  ||
            node->son[1]->type == AST_AND ||
            node->son[1]->type == AST_NOT) {

            fprintf(stderr, "[LINE %d] Semantic Error: Right operand can not be logical.\n", node->line_number);
            exit(4);
        }
    }


	if (node->type == AST_AND || node->type == AST_OR || node->type == AST_NOT) {

		if(node->son[0]->type == AST_SOMA ||
           node->son[0]->type == AST_SUB  ||
           node->son[0]->type == AST_DIV  ||
           node->son[0]->type == AST_MUL) {
			fprintf(stderr, "[LINE %d] Semantic Error: Left operand can not be arithmetic.\n", node->line_number);
			exit(4);
		}

		if(node->son[1]->type == AST_SOMA ||
           node->son[1]->type == AST_SUB  ||
           node->son[1]->type == AST_DIV  ||
           node->son[1]->type == AST_MUL) {
			fprintf(stderr, "[LINE %d] Semantic Error: Right operand can not be arithmetic.\n", node->line_number);
			exit(4);
		}
	}

	for (i=0; i<MAX_SONS; ++i)
		check_operands(node->son[i]);
}


void check_usage(AST *node){
	int i;
    if (!node) return;

	switch(node->type) {
		case AST_ATRIBUICAO:

			//Atribuicao para um identificador que eh ponteiro:
			if (node->symbol->type == SYMBOL_POINTER) {
				//printf("%s\n","Atribuicao de ponteiro");
				verifica_atribuicao_ponteiros(node);
				//printf("Aponta para id = %s\n",node->point_to_symbol->text);

			}
				//Atribuicao para identificador escalar:
			else {
				if (node->symbol->type != SYMBOL_SCALAR) {
					fprintf(stderr, "[LINE %d] ERRO: identificador %s deve ser escalar.\n", node->line_number,
							node->symbol->text);
					exit(4);
				}


				if (node->son[0]->symbol != NULL) {
					if (node->symbol->datatype == DATATYPE_INT && node->son[0]->symbol->datatype == DATATYPE_FLOAT) {
						fprintf(stderr, "[LINE %d] Semantic Error: float can not be converted to int.\n",
								node->line_number);
						exit(4);
					}

					if (node->symbol->datatype == DATATYPE_CHAR && node->son[0]->symbol->datatype == DATATYPE_FLOAT) {
						fprintf(stderr, "[LINE %d] Semantic Error: float can not be converted to char.\n",
								node->line_number);
						exit(4);
					}
				}
				break;


				switch (node->son[0]->type) {
					case AST_IDENT_DERREFERENCIA:
						printf("Derreferencia\n");
						printf("Pegar valor do ponteiro\n");

						//Valor do identificador na hash deve receber o valor
						//do conteudo do ponteiro

						//procura declaracao do ponteiro
						AST *node_decl_pointer = procura_declaracao_ponteiro(nodo_raiz, node->son[0]->symbol->text);

						printf("Nodo declaracao ponteiro id :%s\n", node_decl_pointer->symbol->text);


						if (node_decl_pointer->symbol->type == SYMBOL_POINTER) {

							printf("Simbolo eh um ponteiro\n");
							printf("DATATYPE = %d\n", node_decl_pointer->symbol->datatype);
						}


						printf("Valor a ser atribuido %s\n", node_decl_pointer->son[1]->symbol->text);

						//coloca o valor na entrada hash correspondente ao identificador
						//node->symbol->text = node_decl_pointer->son[1]->symbol->text;


						//Procura a declaracao do identificador
						AST *nodo_decl = procura_declaracao_global(nodo_raiz, node->symbol->text);
						printf("identificador: %s\n", nodo_decl->symbol->text);

						printf("Valor antigo do identificador: %s\n", nodo_decl->son[1]->symbol->text);

						//Coloca novo valor na hash
						nodo_decl->son[1]->symbol->text = node_decl_pointer->son[1]->symbol->text;

						printf("Novo valor do identificador: %s\n", nodo_decl->son[1]->symbol->text);

				}

			}

			break;

		case AST_CHAMADA_FUNCAO:
			if (node->symbol->type != SYMBOL_FUNCTION) {
				fprintf(stderr, "[LINE %d] ERRO: identificador deve ser uma funcao.\n", node->line_number);
				exit(4);
			}

			int qtd_param_chamada = 0;
			int qtd_param_def_funcao = node->symbol->qtd_parametros;

			// conta quantos parametros a a funcao recebeu na sua chamada
			qtd_param_chamada = conta_parametros(node->son[0]);

			if (qtd_param_chamada != qtd_param_def_funcao) {
				fprintf(stderr, "[LINE %d] ERRO: Quantidade de parametros diferentes na chamada da funcao.\n",
						node->line_number);
				exit(4);
			}

			if (qtd_param_chamada != 0) {

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

        case AST_ATRIBUICAO_VETOR: //Atribuição
		case AST_VET: //Expressão

			//Se o indentificador não for vetor
			if (node->symbol->type != SYMBOL_VECTOR) {
				fprintf(stderr, "[LINE %d] Semantic Error: identifier must be a vector.\n", node->line_number);
				exit(4);
			}
			if (node->son[0]->symbol != NULL) {
				//Se o literal não for inteiro ou char
				if ((node->son[0]->symbol->type == SYMBOL_LIT_STRING) ||
					(node->son[0]->symbol->type == SYMBOL_LIT_REAL)) {
					fprintf(stderr, "[LINE %d] Semantic Error: index must be an integer.\n", node->line_number);
					exit(4);
				}
				//Se o identificador não for inteiro ou char
				if ((node->son[0]->symbol->datatype == DATATYPE_FLOAT)) {
					fprintf(stderr, "[LINE %d] Semantic Error: index must be an integer.\n", node->line_number);
					exit(4);
				}
			}

            if (node->symbol->type != AST_SYMBOL) {
                //Se a expressão não retornar um inteiro ou char
                if ((node->son[0]->type != AST_SOMA) && (node->son[0]->type != AST_SUB)) {
                    fprintf(stderr, "[LINE %d] Semantic Error: index must be an integer.\n", node->line_number);
                    exit(4);

                    //Necessário verificar o tipo dos operandos...


                }
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

AST *procura_declaracao_global(AST *node, char *nome)
{
	int i;
	AST *funcao;

	if(!node) return 0;

	if(node->type == AST_DECL_GLOBAL)
		if(node->symbol != NULL){
			if(strcmp(node->symbol->text, nome) == 0)
				return node;
		}

	for(i=0; i < MAX_SONS; i++)
		if(node->son[i] != NULL) {
			funcao = procura_declaracao_global(node->son[i], nome);
			if(funcao)
				return funcao;
		}
	return 0;
}

AST *procura_declaracao_ponteiro(AST *node, char *nome)
{
	int i;
	AST *funcao;

	//printf("Tipo da declaracao: %d\n",node->type);
	if(!node) return 0;

	if(node->type == AST_DECL_PONTEIRO)
		if(node->symbol != NULL){
			if(strcmp(node->symbol->text, nome) == 0){
				//printf("Achou id %s\n",nome );
				printf("Achou declaracao de ponteiro\n");
				return node;

			}
		}


	for(i=0; i < MAX_SONS; i++)
		if(node->son[i] != NULL) {
			funcao = procura_declaracao_ponteiro(node->son[i], nome);
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

void verifica_atribuicao_ponteiros(AST *node){
	if(!node) return;
	AST* nodedef;
	switch (node->son[0]->type) {
		case AST_IDENT_REFERENCIA:
			//printf("%s\n", "Referencia");
			node->point_to_symbol = node->son[0]->symbol;
			break;
	}
}

void derreferencia(AST *node){

}
