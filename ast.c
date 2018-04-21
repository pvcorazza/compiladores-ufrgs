//
// Created by pvcorazza on 4/21/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

AST* astCreate(int type, hash_entry* symbol, AST* son0, AST* son1, AST* son2, AST* son3) {
    AST *node = (AST*) calloc(1, sizeof(AST));
    node->type = type;
    node->symbol = symbol;
    node->son[0] = son0;
    node->son[1] = son1;
    node->son[2] = son2;
    node->son[3] = son3;
    return node;
}

void astPrint(AST *node, int level) {
    if (!node) return;
    int i;
    for (i = 0; i < level; i++) fprintf(stderr, "  ");
    fprintf(stderr, "AST(");
    switch (node->type) {
        case AST_PROGRAMA:
            fprintf(stderr, "AST_PROGRAMA");
            break;
        case AST_CORPO:
            fprintf(stderr, "AST_CORPO");
            break;
        case AST_DECL_GLOBAL:
            fprintf(stderr, "AST_DECL_GLOBAL");
            break;
        case AST_VETOR_INIC:
            fprintf(stderr, "AST_VETOR_INIC");
            break;
        case AST_VETOR_VAZIO:
            fprintf(stderr, "AST_VETOR_VAZIO");
            break;
        case AST_DECL_PONTEIRO:
            fprintf(stderr, "AST_DECL_PONTEIRO");
            break;
        case AST_INT:
            fprintf(stderr, "AST_INT");
            break;
        case AST_FLOAT:
            fprintf(stderr, "AST_FLOAT");
            break;
        case AST_CHAR:
            fprintf(stderr, "AST_CHAR");
            break;
        case AST_DEF_FUNCAO:
            fprintf(stderr, "AST_DEF_FUNCAO");
            break;
        case AST_PARAM_LIST:
            fprintf(stderr, "AST_PARAM_LIST");
            break;
        case AST_BLOCO_COMANDOS:
            fprintf(stderr, "AST_BLOCO_COMANDOS");
            break;
        case AST_COMAND_LIST:
            fprintf(stderr, "AST_COMAND_LIST");
            break;
        case AST_ATRIBUICAO:
            fprintf(stderr, "AST_ATRIBUICAO");
            break;
        case AST_ATRIBUICAO_VETOR:
            fprintf(stderr, "AST_ATRIBUICAO_VETOR");
            break;
        case AST_LISTA_PRINT:
            fprintf(stderr, "AST_LISTA_PRINT");
            break;
        case AST_IDENTIFICADOR:
            fprintf(stderr, "AST_IDENTIFICADOR");
            break;
        case AST_EXP_PARENTESES:
            fprintf(stderr, "AST_EXP_PARENTESES");
            break;
        case AST_SOMA:
            fprintf(stderr, "AST_SOMA");
            break;
        case AST_SUB:
            fprintf(stderr, "AST_SUB");
            break;
        case AST_MUL:
            fprintf(stderr, "AST_MUL");
            break;
        case AST_DIV:
            fprintf(stderr, "AST_DIV");
            break;
        case AST_L:
            fprintf(stderr, "AST_L");
            break;
        case AST_G:
            fprintf(stderr, "AST_G");
            break;
        case AST_LE:
            fprintf(stderr, "AST_LE");
            break;
        case AST_GE:
            fprintf(stderr, "AST_GE");
            break;
        case AST_EQ:
            fprintf(stderr, "AST_EQ");
            break;
        case AST_NE:
            fprintf(stderr, "AST_NE");
            break;
        case AST_AND:
            fprintf(stderr, "AST_AND");
            break;
        case AST_OR:
            fprintf(stderr, "AST_OR");
            break;
        case AST_IDENT_REFERENCIA:
            fprintf(stderr, "AST_IDENT_REFERENCIA");
            break;
        case AST_IDENT_DERREFERENCIA:
            fprintf(stderr, "AST_IDENT_DERREFERENCIA");
            break;
        case AST_ARG_FUNCAO:
            fprintf(stderr, "AST_ARG_FUNCAO");
            break;
        case AST_IF:
            fprintf(stderr, "AST_IF");
            break;
        case AST_IF_ELSE:
            fprintf(stderr, "AST_IF_ELSE");
            break;
        case AST_FOR:
            fprintf(stderr, "AST_FOR");
            break;
        case AST_READ:
            fprintf(stderr, "AST_READ");
            break;
        case AST_RETURN:
            fprintf(stderr, "AST_RETURN");
            break;
        case AST_WHILE:
            fprintf(stderr, "AST_WHILE");
            break;
        case AST_PRINT:
            fprintf(stderr, "AST_PRINT");
            break;
        case AST_SYMBOL:
            fprintf(stderr, "AST_SYMBOL");
            break;
        case AST_NOT:
            fprintf(stderr, "AST_NOT");
            break;
        case AST_PARAM:
            fprintf(stderr, "AST_PARAM");
            break;
        case AST_LISTA_INIC:
            fprintf(stderr, "AST_LISTA_INIC");
            break;
        default:
            fprintf(stderr, "Node Type: %d", node->type);
            break;
    }

    if (node->symbol) {
        fprintf(stderr, ", %s)\n", node->symbol->text);
    } else {
        fprintf(stderr, ",)\n");
    }

    for (i = 0; i < MAX_SONS; i++) {
        astPrint(node->son[i], level + 1);
    }
}

    FILE *out;

    void uncompile(AST *ast){

        if(!ast) return;
        if(ast){

            switch(ast->type){

                case AST_SYMBOL:
                    printf("%s", ast->symbol->text);
                    break;
                case AST_CORPO:
                    if(ast->son[1] != NULL) {
                        uncompile(ast->son[0]);
                        printf(" \n");
                        uncompile(ast->son[1]);
                    }
                    else
                        uncompile(ast->son[0]);
                    break;
                case AST_DECL_GLOBAL:
                    uncompile(ast->son[0]);
                    printf(" %s = ", ast->symbol->text);
                    uncompile(ast->son[1]);
                    printf(";");
                    break;
                case AST_VETOR_INIC:
                    uncompile(ast->son[0]);
                    printf(" %s [", ast->symbol->text);
                    uncompile(ast->son[1]);
                    printf("] : ");
                    uncompile(ast->son[2]);
                    printf(";");



                    break;
                case AST_VETOR_VAZIO: uncompile(ast->son[0]); break;
                case AST_DECL_PONTEIRO: fprintf(stderr, "AST_DECL_PONTEIRO"); break;
                case AST_INT: printf("int");
                    break;
                case AST_FLOAT:
                    fprintf(out,"float");
                    break;
                case AST_CHAR: fprintf(stderr, "AST_CHAR"); break;
                case AST_DEF_FUNCAO: fprintf(stderr, "AST_DEF_FUNCAO"); break;
                case AST_PARAM_LIST: fprintf(stderr, "AST_PARAM_LIST"); break;
                case AST_BLOCO_COMANDOS: fprintf(stderr, "AST_BLOCO_COMANDOS"); break;
                case AST_COMAND_LIST: fprintf(stderr, "AST_COMAND_LIST"); break;
                case AST_ATRIBUICAO: fprintf(stderr, "AST_ATRIBUICAO"); break;
                case AST_ATRIBUICAO_VETOR: fprintf(stderr, "AST_ATRIBUICAO_VETOR"); break;
                case AST_LISTA_PRINT: fprintf(stderr, "AST_LISTA_PRINT"); break;
                case AST_IDENTIFICADOR: fprintf(stderr, "AST_IDENTIFICADOR"); break;
                case AST_EXP_PARENTESES: fprintf(stderr, "AST_EXP_PARENTESES"); break;
                case AST_SOMA:
                    uncompile(ast->son[0]); fprintf(out,"+"); uncompile(ast->son[1]);
                    break;
                case AST_SUB:
                    uncompile(ast->son[0]); fprintf(out,"-"); uncompile(ast->son[1]);
                    break;
                case AST_MUL:
                    uncompile(ast->son[0]); fprintf(out,"*"); uncompile(ast->son[1]);
                    break;
                case AST_DIV:
                    uncompile(ast->son[0]); fprintf(out,"/"); uncompile(ast->son[1]);
                    break;
                case AST_L:
                    uncompile(ast->son[0]); fprintf(out,"<"); uncompile(ast->son[1]);
                    break;
                case AST_G:
                    uncompile(ast->son[0]); fprintf(out,">"); uncompile(ast->son[1]);
                    break;
                case AST_LE:
                    uncompile(ast->son[0]); fprintf(out,"<="); uncompile(ast->son[1]);
                    break;
                case AST_GE:
                    uncompile(ast->son[0]); fprintf(out,">="); uncompile(ast->son[1]);
                    break;
                case AST_EQ:
                    uncompile(ast->son[0]); fprintf(out,"=="); uncompile(ast->son[1]);
                    break;
                case AST_NE:
                    uncompile(ast->son[0]); fprintf(out,"!="); uncompile(ast->son[1]);
                    break;
                case AST_AND:
                    uncompile(ast->son[0]); fprintf(out,"&&"); uncompile(ast->son[1]);
                    break;
                case AST_OR:
                    uncompile(ast->son[0]); fprintf(out,"||"); uncompile(ast->son[1]);
                    break;

                case AST_IDENT_REFERENCIA: fprintf(stderr, "AST_IDENT_REFERENCIA"); break;
                case AST_IDENT_DERREFERENCIA: fprintf(stderr, "AST_IDENT_DERREFERENCIA"); break;
                case AST_ARG_FUNCAO: fprintf(stderr, "AST_ARG_FUNCAO"); break;
                case AST_IF:
                    fprintf(out,"if("); uncompile(ast->son[0]); fprintf(out,") then ");fprintf(out," \n"); uncompile(ast->son[1]);
                    break;
                case AST_IF_ELSE: fprintf(stderr, "AST_IF_ELSE"); break;
                case AST_FOR: fprintf(stderr, "AST_FOR"); break;

                case AST_READ:
                    fprintf(out,"read > "); fprintf(out,"%s", ast->symbol->text);
                    break;
                case AST_RETURN:
                    fprintf(out,"return ");
                    uncompile(ast->son[0]);
                    break;
                case AST_WHILE:
                    fprintf(out,"while(");
                    uncompile(ast->son[0]);
                    fprintf(out,")");
                    uncompile(ast->son[1]);
                    break;
                case AST_PRINT:
                    fprintf(out,"print "); uncompile(ast->son[0]);
                    break;

                case AST_NOT:
                    fprintf(out,"!");uncompile(ast->son[0]);
                    break;

                case AST_PARAM:
                    fprintf(out,"%s:", ast->symbol->text); uncompile(ast->son[0]);
                    break;

                case AST_LISTA_INIC:
                    uncompile(ast->son[0]);
                    printf(" ");
                    uncompile(ast->son[1]);
                    break;









//                case AST_FUNDEF:
//                    fprintf(out,"("); uncompile(ast->son[0]); fprintf(out,")"); fprintf(out,"%s", ast->symbol->text); fprintf(out,"("); uncompile(ast->son[1]); fprintf(out,")"); uncompile(ast->son[2]);
//                    break;
//                case AST_FUNCALL:
//                    fprintf(out,"%s", ast->symbol->text); fprintf(out,"("); uncompile(ast->son[0]); fprintf(out,")");
//                    break;
//                case AST_PARENTHESIS:
//                    fprintf(out,"("); uncompile(ast->son[0]); fprintf(out,")");
//                    break;
//                case AST_BRACES:
//                    fprintf(out,"{\n"); uncompile(ast->son[0]); fprintf(out,"}");
//                    break;
//
//                case AST_ATR:
//                    fprintf(out,"%s", ast->symbol->text); fprintf(out,"="); uncompile(ast->son[0]);
//                    break;
//                case AST_AATR:
//                    fprintf(out, "%s", ast->symbol->text); fprintf(out, "["); uncompile(ast->son[0]);fprintf(out, "]"); fprintf(out, "="); uncompile(ast->son[1]);
//                    break;
//                case AST_AACC:
//                    fprintf(out,"%s", ast->symbol->text); fprintf(out,"["); uncompile(ast->son[0]); fprintf(out,"]");
//                    break;
//
//                case AST_PRINTL:
//                    if(ast->son[1] != NULL){
//                        uncompile(ast->son[0]); fprintf(out,","); uncompile(ast->son[1]);
//                    }else
//                        uncompile(ast->son[0]);
//                    break;
//                case AST_CMDL:
//                    if(ast->son[1] != NULL){
//                        uncompile(ast->son[0]); fprintf(out,";\n"); uncompile(ast->son[1]);
//                    }else
//                        uncompile(ast->son[0]);
//                    break;
//                case AST_DFNL:
//
//                    if(ast->son[1] != NULL)
//                    {
//                        uncompile(ast->son[0]); fprintf(out," \n"); uncompile(ast->son[1]);
//                    }
//                    else
//                        uncompile(ast->son[0]);
//
//                    break;
//                case AST_PARL:
//                    if(ast->son[1] != NULL){
//                        uncompile(ast->son[0]); fprintf(out,","); uncompile(ast->son[1]);
//                    }else
//                        uncompile(ast->son[0]);
//                    break;
//                case AST_INTL:
//                    if(ast->son[1] != NULL){
//                        uncompile(ast->son[0]); fprintf(out," "); uncompile(ast->son[1]);
//                    }else
//                        uncompile(ast->son[0]);
//                    break;
//                case AST_VARINI:
//                    fprintf(out,"%s", ast->symbol->text); fprintf(out,": "); uncompile(ast->son[0]); fprintf(out,"="); uncompile(ast->son[1]);fprintf(out,";"); uncompile(ast->son[2]);
//                    break;
//                case AST_BYTE:
//                    fprintf(out,"byte");
//                    break;
//                case AST_SHORT:
//                    fprintf(out,"short");
//                    break;
//                case AST_LONG:
//                    fprintf(out,"long");
//                    break;
//
//                case AST_DOUBLE:
//                    fprintf(out,"double");
//                    break;
//
//                case AST_ELSE:
//                    fprintf(out,"if("); uncompile(ast->son[0]); fprintf(out,") then "); uncompile(ast->son[1]);fprintf(out,"else "); uncompile(ast->son[1]);
//                    break;
//                case AST_ARRINI:
//                    fprintf(out,"%s", ast->symbol->text); fprintf(out,": "); uncompile(ast->son[0]); fprintf(out,"["); 	uncompile(ast->son[1]); fprintf(out,"]");
//
//                    if(ast->son[2]!=NULL)
//                        uncompile(ast->son[2]);
//                    fprintf(out,";");
//                    break;
                default:
                    printf("ERRO\n");
                    break;
            }
        }

        return;

    }