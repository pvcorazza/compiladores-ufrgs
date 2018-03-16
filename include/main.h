/*
  main.h

  Cabeçalho principal do analisador sintático
*/
#ifndef __MAIN_H
#define __MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include "cc_dict.h"
#include "cc_list.h"
#include "cc_misc.h"
#include "cc_tree.h"
#include "cc_gv.h"
#include "parser.h"

/*
  Protótipos de funções chamadas pelo flex
*/
int yylex();
void yyerror(const char *s);

/*
  Constantes a serem utilizadas como valor de retorno no caso de
  sucesso (SINTATICA_SUCESSO) e erro (SINTATICA_ERRO) do analisador
  sintático.
*/
#define SINTATICA_SUCESSO 0
#define SINTATICA_ERRO    1

/*
  Constantes a serem utilizadas para diferenciar os lexemas que estão
  registrados na tabela de símbolos.
*/
#define SIMBOLO_LITERAL_INT    1
#define SIMBOLO_LITERAL_FLOAT  2
#define SIMBOLO_LITERAL_CHAR   3
#define SIMBOLO_LITERAL_STRING 4
#define SIMBOLO_LITERAL_BOOL   5
#define SIMBOLO_IDENTIFICADOR  6

void cc_dict_etapa_1_print_entrada (char *token, int line);
int comp_get_line_number (void);
void comp_print_table (void);

#endif
