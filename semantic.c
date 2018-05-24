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
                case AST_DEF_FUNCAO: node->symbol->type = SYMBOL_FUNCTION;
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

                if ((node->son[0]->symbol->datatype == DATATYPE_INT) || (node->son[0]->symbol->datatype == DATATYPE_FLOAT) || (node->son[0]->symbol->datatype == DATATYPE_CHAR)) {
                    expr1 = node->son[0]->symbol->datatype;
                } else {
                    if (node->son[0]->type == AST_IDENT_DERREFERENCIA)
                        expr1 = node->son[0]->symbol->datatype; //pega o tipo do ponteiro
                    else
                        expr1 = node->son[0]->symbol->type;
                }
				//printf("Expr1 %d\n", expr1);
			} else {
				set_expression_datatypes(node->son[0]);
				expr1 = node->son[0]->expression_datatype;
			}

			if (node->son[1]->symbol != NULL) {

                if ((node->son[1]->symbol->datatype != DATATYPE_INT) || (node->son[1]->symbol->datatype != DATATYPE_FLOAT) ||(node->son[1]->symbol->datatype != DATATYPE_CHAR)) {
                    expr2 = node->son[1]->symbol->datatype;
                } else {
                    if (node->son[1]->type == AST_IDENT_DERREFERENCIA)
                        expr2 = node->son[1]->symbol->datatype; //pega o tipo do ponteiro
                    else
                        expr2 = node->son[1]->symbol->type;
                }
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

        case AST_DEF_FUNCAO:
			check_return(node->son[2], node);
			if (node->return_datatype !=0) {
            if (node->symbol->datatype != node->return_datatype) {
				
					fprintf(stderr, "[LINE %d] Semantic Error: return for the function %s() has not declared.\n", node->line_number, node->symbol->text);
				
				error++;
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
					fprintf(stderr, "[LINE %d] Semantic Error: id %s must be a scalar.\n", node->line_number,
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

				AST *node_decl_pointer;
				switch (node->son[0]->type) {
					case AST_IDENT_DERREFERENCIA:

						//procura declaracao do ponteiro
						node_decl_pointer = procura_declaracao_ponteiro(nodo_raiz, node->son[0]->symbol->text);

						//Procura a declaracao do identificador
						AST *nodo_decl = procura_declaracao_global(nodo_raiz, node->symbol->text);


						int tipo_identificador = nodo_decl->symbol->datatype;
						int tipo_do_ponteiro = node_decl_pointer->symbol->datatype;

                        if (tipo_identificador == DATATYPE_FLOAT || tipo_do_ponteiro == DATATYPE_FLOAT) {
                            if (tipo_identificador != tipo_do_ponteiro) {
                                fprintf(stderr, "[LINE %d] Semantic Error: incompatible types.\n",
                                        node->line_number);
                                error++;
                            }
                        }
                        nodo_decl->son[1]->symbol->text = node_decl_pointer->son[1]->symbol->text;
                        
                        break;
                    case AST_SYMBOL:

						if(node->son[0]->symbol->type == SYMBOL_VECTOR) {
							fprintf(stderr, "[LINE %d] Semantic Error: Assignment to a scalar with a vector symbol.\n",
									node->line_number);
							exit(4);
						}
						
                        break;

						/*
						if (tipo_identificador != NO_EXPRESSION) {

							if (tipo_identificador != node_decl_pointer->expression_datatype) {
								fprintf(stderr, "[LINE %d] Semantic Error: incompatible types (expression).\n",
										node->line_number);
								error++;
							}
						}*/

						//printf("Atribui ao identificador \"%s\" ", nodo_decl->symbol->text);
						//printf("cujo valor antigo é: %s ", nodo_decl->son[1]->symbol->text);
						//Coloca novo valor na hash
						//nodo_decl->son[1]->symbol->text = node_decl_pointer->son[1]->symbol->text;
						//printf("o novo valor: %s ", nodo_decl->son[1]->symbol->text);
						//printf("que veio do ponteiro :\"%s\"\n", node_decl_pointer->symbol->text);


				}

			}

			break;

		case AST_CHAMADA_FUNCAO:
			if (node->symbol->type != SYMBOL_FUNCTION) {
				fprintf(stderr, "[LINE %d] Semantic Error: id must be a function.\n", node->line_number);
				error++;
			}

			verifica_tipos_parametros_funcao(node);

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
				if (node->son[0]->symbol->datatype == DATATYPE_FLOAT) {
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
	int nodecall_line = nodecall->line_number;
	int tipo_argumento_decl=0,tipo_argumento_call=0;
	if(!nodecall) return;
	AST* nodedef;
	if(nodecall->symbol != NULL) nodedef = procura_def_funcao(nodo_raiz, nodecall->symbol->text);

	nodecall = nodecall->son[0];
	while(nodedef != NULL){

        if(nodecall == NULL){
			fprintf(stderr, "[LINE %d] Semantic Error: Declaration and call funtions must have the same quantity of parameters\n",nodecall_line);
			exit(4);
        }

		if(nodedef->type == AST_PARAM_LIST){
			//printf("Lista de parametros\n");

			if(nodecall->son[0] == NULL){
				fprintf(stderr, "[LINE %d] Semantic Error: Declaration and call funtions must have the same quantity of parameters\n",nodecall_line);
				exit(4);
			}

			if(nodedef->son[0]->type == AST_PARAM){
				//printf("Funcao com 1 parametro\n");
                //printf("Node call: %d\n",nodecall->son[0]->symbol->datatype );
				//printf("Node def : %d\n",nodedef->son[0]->symbol->datatype);

				tipo_argumento_decl = nodedef->son[0]->symbol->datatype;

                if(nodecall->son[0]->expression_datatype != NO_EXPRESSION){
                    //printf("Eh uma expressao\n");
                    //printf("Tipo da expressao %d\n",nodecall->son[0]->expression_datatype );
					tipo_argumento_call = nodecall->son[0]->expression_datatype;
				}
                else{
                    //printf("Arg call type %d\n", nodecall->son[0]->symbol->datatype);
					tipo_argumento_call = nodecall->son[0]->symbol->datatype;

                }



                nodecall = nodecall->son[1]; //proximo argumento da chamada de funcao

            }

			if(nodedef->son[1]->type == AST_PARAM){
                //printf("Ultimo paramentro\n");

				//printf("nodecal type = %d\n",nodecall->symbol->datatype);
				//printf("Node def : %d\n",nodedef->son[1]->symbol->datatype);


				if(nodecall->type==AST_ARG_FUNCAO){
                    fprintf(stderr, "[LINE %d] Semantic Error: Declaration and call funtions must have the same quantity of parameters\n",
                            nodecall_line);

                    exit(4);
				}

				tipo_argumento_decl = nodedef->son[1]->symbol->datatype;


				if(nodecall->expression_datatype != NO_EXPRESSION){
                    //printf("Eh uma expressao\n");
                    //printf("Tipo da expressao %d\n",nodecall->expression_datatype );
					tipo_argumento_call = nodecall->expression_datatype;

				}
                else{
                    //printf("Arg call type %d\n", nodecall->symbol->datatype);
					tipo_argumento_call = nodecall->symbol->datatype;


				}


			}
		}



		if(tipo_argumento_decl != tipo_argumento_call){
			fprintf(stderr, "[LINE %d] Semantic Error: Arguments at function call must have the same type as declared in function header.\n",
					nodecall_line);
			exit(4);
		}


		nodedef = nodedef->son[1]; //proximo parametro na declaracao da funcao

	}


}

void verifica_atribuicao_ponteiros(AST *node){
	if(!node) return;
	switch (node->son[0]->type) {
		case AST_IDENT_REFERENCIA:
			node->point_to_symbol = node->son[0]->symbol;
			break;
	}
}


void check_return(AST* node, AST* function_node) {
    if(!node) return;
    if(node->type == AST_RETURN) {

        int type = check_datatype(node);
		function_node->return_datatype = type;
    }

    for(int i=0; i<MAX_SONS; i++) {
        check_return(node->son[i], function_node);
    }
}

int check_datatype(AST* node) {

	if (node->son[0]->symbol != NULL) {
		return node->son[0]->symbol->datatype;
	}

	if (node->son[0]->expression_datatype != NO_EXPRESSION) {
		return node->son[0]->expression_datatype;
	}
	return NO_EXPRESSION;
}
