//
// Created by pvcorazza on 5/18/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

int error = 0;
AST *nodo_raiz;
void semantic_analisys (AST *node) {

	//guarda a raiz recebida
	nodo_raiz = node;
    set_declarations(node);
	set_expression_datatypes(node);
    check_undeclared(node);
    check_operands(node);
	check_usage(node);
	if (error > 0) {
        fprintf(stderr, "Compilation failed!\n");
		exit(4);
	}
}


void set_declarations(AST *node) {

    int i;
    if (!node) return;

    //Verifica declarações globais
    if (node->type == AST_DECL_GLOBAL || node->type == AST_VETOR_VAZIO ||
		node->type == AST_VETOR_INIC || node->type == AST_DECL_PONTEIRO ||
		node->type == AST_DEF_FUNCAO) {
        if (node->symbol->type != SYMBOL_IDENTIFIER) {
            fprintf(stderr, "[LINE %d] Semantic Error: Symbol '%s' already declared.\n",  node->line_number, node->symbol->text);
            error++;
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

	if (node->type == AST_SYMBOL) {
		if (node->symbol->type == SYMBOL_LIT_INT) node->symbol->datatype = DATATYPE_INT;
		if (node->symbol->type == SYMBOL_LIT_REAL) node->symbol->datatype = DATATYPE_FLOAT;
		if (node->symbol->type == SYMBOL_LIT_CHAR) node->symbol->datatype = DATATYPE_CHAR;
	}


	//Verifica declaracao dos parametros da funcao
	if(node->type == AST_PARAM){
		if (node->symbol->type != SYMBOL_IDENTIFIER) {
            fprintf(stderr, "[LINE %d] Semantic Error: Symbol '%s' already declared.\n",  node->line_number, node->symbol->text);
            error++;
        }
		else{

			node->symbol->type = SYMBOL_SCALAR;

			if (node->son[0]->type == AST_INT) node->symbol->datatype = DATATYPE_INT;
            if (node->son[0]->type == AST_FLOAT) node->symbol->datatype = DATATYPE_FLOAT;
            if (node->son[0]->type == AST_CHAR) node->symbol->datatype = DATATYPE_CHAR;
		}
	}


	if(node->type == AST_IDENT_DERREFERENCIA){
		printf("AST_IDENT_DERREFERENCIA\n");

		printf("Identificador %s\n",node->symbol->text);
		AST *node_decl_pointer = procura_declaracao_ponteiro(nodo_raiz, node->symbol->text);
		printf("Tipo do identificador declarado: %d\n",node->symbol->datatype);

		//printf("Symbol type %d\n", node->son[0]->type);

		//node->son[0]-

		//exit(4);
	}

    for (i = 0; i < MAX_SONS; ++i) {
        set_declarations(node->son[i]);
    }
}

//Verifica se simbolos ainda não foram declarados
void check_undeclared(AST *node){
    if(!node) return;
    for (int i=0; i<MAX_SONS; i++) {
        check_undeclared(node->son[i]);
    }

    if(node->symbol != 0 && node->symbol->type == SYMBOL_IDENTIFIER){
        fprintf(stderr, "[LINE %d] Semantic Error: Symbol '%s' is not declared.\n",  node->line_number, node->symbol->text);
        error++;
    }
}

void set_expression_datatypes(AST *node) {
	int i;
	if (!node) return;
	int expr1;
	int expr2;
	switch (node->type) {

        case AST_EXP_PARENTESES:
            set_expression_datatypes(node->son[0]);
            node->expression_datatype = node->son[0]->expression_datatype;
            break;
		case AST_SOMA:
		case AST_SUB:
		case AST_MUL:
		case AST_DIV:
		case AST_LE:
		case AST_GE:
		case AST_EQ:
		case AST_NE:
		case AST_AND:
		case AST_OR:
		case AST_G:
		case AST_L:
		case AST_NOT:

			if (node->son[0]->symbol != NULL) {

				if(node->son[0]->type == AST_IDENT_DERREFERENCIA)
					expr1 = node->son[0]->symbol->datatype; //pega o tipo do ponteiro
				else
					expr1 = node->son[0]->symbol->type;
				//printf("Expr1 %d\n", expr1);
			} else {
				set_expression_datatypes(node->son[0]);
				expr1 = node->son[0]->expression_datatype;
			}

			if (node->son[1]->symbol != NULL) {
				if(node->son[1]->type == AST_IDENT_DERREFERENCIA)
					expr2 = node->son[1]->symbol->datatype; //pega o tipo do ponteiro
				else
					expr2 = node->son[1]->symbol->type;
				//printf("Expr2 %d\n", expr2);
			} else {
				set_expression_datatypes(node->son[1]);
				expr2 = node->son[1]->expression_datatype;
			}

			if (expr1 >= 0 && expr2 >= 0) {
				if ((expr1 == SYMBOL_LIT_REAL) || (expr2 == SYMBOL_LIT_REAL) || (expr1 == DATATYPE_FLOAT) || (expr2 == DATATYPE_FLOAT)) {
					node->expression_datatype = DATATYPE_FLOAT;
				} else {
					if ((expr1 == SYMBOL_LIT_INT) || (expr2 == SYMBOL_LIT_INT) || (expr1 == DATATYPE_INT) || (expr2 == DATATYPE_INT)) {
						node->expression_datatype = DATATYPE_INT;
					} else {
						if ((expr1 == SYMBOL_LIT_CHAR) || (expr2 == SYMBOL_LIT_CHAR)) {
							node->expression_datatype = DATATYPE_INT;
						}
					}
				}
			}

			break;

		default:
			node->expression_datatype = NO_EXPRESSION;

	}

	for (i=0; i<MAX_SONS; ++i)
		set_expression_datatypes(node->son[i]);
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
            error++;
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
            error++;
        }
    }


	if (node->type == AST_AND || node->type == AST_OR || node->type == AST_NOT) {

		if(node->son[0]->type == AST_SOMA ||
           node->son[0]->type == AST_SUB  ||
           node->son[0]->type == AST_DIV  ||
           node->son[0]->type == AST_MUL) {
			fprintf(stderr, "[LINE %d] Semantic Error: Left operand can not be arithmetic.\n", node->line_number);
			error++;
		}

		if(node->son[1]->type == AST_SOMA ||
           node->son[1]->type == AST_SUB  ||
           node->son[1]->type == AST_DIV  ||
           node->son[1]->type == AST_MUL) {
			fprintf(stderr, "[LINE %d] Semantic Error: Right operand can not be arithmetic.\n", node->line_number);
			error++;
		}
	}

	for (i=0; i<MAX_SONS; ++i)
		check_operands(node->son[i]);
}


void check_usage(AST *node){
	int i;
    if (!node) return;

	switch(node->type) {

		//Testa valores de inicialização nas declarações globais
		case AST_DECL_GLOBAL:
		case AST_DECL_PONTEIRO:
			if (node->son[1]->symbol != NULL) {
                if (node->symbol->datatype == DATATYPE_FLOAT || node->son[1]->symbol->datatype == DATATYPE_FLOAT) {
                    if (node->symbol->datatype != node->son[1]->symbol->datatype) {
                        fprintf(stderr, "[LINE %d] Semantic Error: incompatible types.\n",
                                node->line_number);
                        error++;
                    }
                }
			}
			break;

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
					error++;
				}

				if (node->son[0]->symbol != NULL) {
                    if (node->symbol->datatype == DATATYPE_FLOAT || node->son[0]->symbol->datatype == DATATYPE_FLOAT) {
                        if (node->symbol->datatype != node->son[0]->symbol->datatype) {
                            fprintf(stderr, "[LINE %d] Semantic Error: incompatible types.\n",
                                    node->line_number);
                            error++;
                        }
                    }
				}

				if (node->son[0]->expression_datatype != NO_EXPRESSION) {

					if (node->symbol->datatype != node->son[0]->expression_datatype) {
						fprintf(stderr, "[LINE %d] Semantic Error: incompatible types (expression).\n",
								node->line_number);
						error++;
					}
				}

				switch (node->son[0]->type) {
					case AST_IDENT_DERREFERENCIA:
						printf("Derreferencia\n");

						//Valor do identificador na hash deve receber o valor
						//do conteudo do ponteiro

						//procura declaracao do ponteiro
						AST *node_decl_pointer = procura_declaracao_ponteiro(nodo_raiz, node->son[0]->symbol->text);

						//Procura a declaracao do identificador
						AST *nodo_decl = procura_declaracao_global(nodo_raiz, node->symbol->text);


						/*
						if (node_decl_pointer->symbol->type == SYMBOL_POINTER) {
							printf("Simbolo eh um ponteiro\n");
							printf("DATATYPE = %d\n", node_decl_pointer->symbol->datatype);
						}*/

						//coloca o valor na entrada hash correspondente ao identificador
						//node->symbol->text = node_decl_pointer->son[1]->symbol->text;

						int tipo_identificador = nodo_decl->symbol->datatype;
						int tipo_do_ponteiro = node_decl_pointer->symbol->datatype;

                        if (tipo_identificador == DATATYPE_FLOAT || tipo_do_ponteiro == DATATYPE_FLOAT) {
                            if (tipo_identificador != tipo_do_ponteiro) {
                                fprintf(stderr, "[LINE %d] Semantic Error: incompatible types.\n",
                                        node->line_number);
                                error++;
                            }
                        }

						/*
						if (tipo_identificador != NO_EXPRESSION) {

							if (tipo_identificador != node_decl_pointer->expression_datatype) {
								fprintf(stderr, "[LINE %d] Semantic Error: incompatible types (expression).\n",
										node->line_number);
								error++;
							}
						}*/

						printf("Atribui ao identificador \"%s\" ", nodo_decl->symbol->text);
						printf("cujo valor antigo é: %s ", nodo_decl->son[1]->symbol->text);
						//Coloca novo valor na hash
						nodo_decl->son[1]->symbol->text = node_decl_pointer->son[1]->symbol->text;
						printf("o novo valor: %s ", nodo_decl->son[1]->symbol->text);
						printf("que veio do ponteiro :\"%s\"\n", node_decl_pointer->symbol->text);



						break;

				}

			}

			break;

		case AST_CHAMADA_FUNCAO:
			if (node->symbol->type != SYMBOL_FUNCTION) {
				fprintf(stderr, "[LINE %d] ERRO: identificador deve ser uma funcao.\n", node->line_number);
				error++;
			}

			int qtd_param_chamada = 0;
			int qtd_param_def_funcao = node->symbol->qtd_parametros;

			// conta quantos parametros a a funcao recebeu na sua chamada
			qtd_param_chamada = conta_parametros(node->son[0]);

			if (qtd_param_chamada != qtd_param_def_funcao) {
				fprintf(stderr, "[LINE %d] ERRO: Quantidade de parametros diferentes na chamada da funcao.\n",
						node->line_number);
				error++;
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
						error++;
					}

					//percorre a lista de parametros (descendo a arvore)
					node = node->son[1];
					nodo_decl_funcao = nodo_decl_funcao->son[1];
				}

			}

			*/

			break;

        case AST_ATRIBUICAO_VETOR: //Atribuição
            if (node->son[1]->symbol != NULL) {
                if (node->symbol->datatype == DATATYPE_FLOAT || node->son[1]->symbol->datatype == DATATYPE_FLOAT) {
                    if (node->symbol->datatype != node->son[1]->symbol->datatype) {
                        fprintf(stderr, "[LINE %d] Semantic Error: incompatible types.\n",
                                node->line_number);
                        error++;
                    }
                }
            }
			if (node->son[1]->expression_datatype != NO_EXPRESSION) {

				if (node->symbol->datatype != node->son[1]->expression_datatype) {
					fprintf(stderr, "[LINE %d] Semantic Error: incompatible types (expression).\n",
							node->line_number);
					error++;
				}
			}
		case AST_VET: //Expressão

			//Se o indentificador não for vetor
			if (node->symbol->type != SYMBOL_VECTOR) {
				fprintf(stderr, "[LINE %d] Semantic Error: identifier must be a vector.\n", node->line_number);
				error++;
			}
			if (node->son[0]->symbol != NULL) {
				//Se o literal não for inteiro ou char
				if ((node->son[0]->symbol->type == SYMBOL_LIT_STRING) ||
					(node->son[0]->symbol->type == SYMBOL_LIT_REAL)) {
					fprintf(stderr, "[LINE %d] Semantic Error: index must be an integer.\n", node->line_number);
					error++;
				}
				//Se o identificador não for inteiro ou char
				if ((node->son[0]->symbol->datatype == DATATYPE_FLOAT)) {
					fprintf(stderr, "[LINE %d] Semantic Error: index must be an integer.\n", node->line_number);
					error++;
				}
			}

            if (node->son[0]->type != AST_SYMBOL) {

				if (node->son[0]->expression_datatype != NO_EXPRESSION) {

					if (node->son[0]->expression_datatype != DATATYPE_INT) {
						fprintf(stderr, "[LINE %d] Semantic Error: index must be an integer.\n", node->line_number);
						error++;
					}
				}
            }

			break;

        case AST_PRINT:
            if(node->son[0]->symbol != NULL) {
                if(node->son[0]->symbol->type != SYMBOL_LIT_STRING) {
                    fprintf(stderr, "[LINE %d] Semantic Error: only strings are accepted in print command.\n", node->line_number);
                    error++;
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
			if(strcmp(node->symbol->text, nome) == 0)
				return node;
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
						error++;
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
