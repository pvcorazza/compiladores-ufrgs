#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define ERRO(MENSAGEM) { fprintf (stderr, "[cc_dict, %s] %s.\n", __FUNCTION__, MENSAGEM); abort(); }

// one-at-a-time-hash, Jenkins, 2006
int generate_hash(char *key, int limit)
{
    unsigned int hash;
    for (hash = 0; *key != '\0'; ++key) {
        hash += *key;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % limit;
}

static table_entry_t *symbol_table_entry_new()
{
  table_entry_t *item = malloc(sizeof(table_entry_t));
  item->key = NULL;
  item->value = NULL;

  return item;
}


static int table_entry_insert(table_entry_t * target,
                            table_entry_t * new_entry)
{
    if (target == NULL)
        return 0;
    while (target->next != NULL)
        target = target->next;
    target->next = new_entry;

    return 1;
}

/*
	Retorna uma entrada da tabela de simbolos caso existir
*/
static table_entry_t *table_entry_get(table_entry_t * first, char *key)
{
    while (first != NULL && first->key != NULL) {
        if (strcmp(first->key, key) == 0)
            return first;
        first = first->next;
    }
    return NULL;
}

static int table_item_list_print(table_entry_t * entry)
{
    int qtd = 0;
    while (entry != NULL) {
        ++qtd;
        printf(" %s %p\n", entry->key, entry->value);
        entry = entry->next;
    }
    return qtd;
}

void debug_print_symbol_table(symbol_table_t * table)
{
    int i, l;
    int qtd = 0;
    printf("table [%d/%d]\n", table->occupation, table->size);
    for (i = 0, l = table->size; i < l; ++i) {
        if (table->data[i]) {
            printf("%d: %s %d\n", i, table->data[i]->key, table->data[i]->tipo);
            ++qtd;
            if (table->data[i]->next)
                qtd += table_item_list_print(table->data[i]->next);
        }
    }
}

symbol_table_t *symbol_table_new(void)
{
    symbol_table_t *table = malloc(sizeof(symbol_table_t));
    if (!table) {
        ERRO("Cannot alocate memory for table");
        return NULL;
    }
    table->size = DICT_SIZE;
    table->data = malloc(sizeof(table_entry_t *) * table->size);
    if (!table->data) {
        free(table);
        ERRO("Cannot alocate memory for table data");
        return NULL;
    }
    table->next = NULL;
    table->prev = NULL;

    int i, l = table->size;
    for (i = 0; i < l; ++i)
        table->data[i] = NULL;
    return table;
}

/*
* Função  para incluir uma nova entrada na tabela de simbolos
*/
table_entry_t * add_symbol_table_entry(symbol_table_t * symbol_table, char *key, int linha)
{

    if (symbol_table == NULL || symbol_table->data == NULL || key == NULL) {
        ERRO("At least one parameter is NULL");
    }

    int hash = generate_hash(key, symbol_table->size);

    table_entry_t *newitem = symbol_table_entry_new();
    newitem->key = strdup(key);
    newitem->linha = linha;

    if (symbol_table->data[hash] == NULL) {       // caso 1: entrada não existe na tabela, é inserido imediatamente
        symbol_table->data[hash] = newitem;

        ++symbol_table->occupation;
    } else {                      // caso 2: entrada existe na tabela, inserir no encadeamento
        table_entry_t *exists = table_entry_get(symbol_table->data[hash], key);
        if (!exists) {
            table_entry_insert(symbol_table->data[hash], newitem);
        } else {
            //atualiza a linha em que foi achado o lexema que ja esta na tabela
            symbol_table->data[hash]->linha = linha;
            //symbol_table(newitem);
            return exists;
        }
    }
    return newitem;
}
