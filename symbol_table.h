#define SIZE 997

#define SYMBOL_LIT_INT 401
#define SYMBOL_LIT_REAL 402
#define SYMBOL_LIT_CHAR 403
#define SYMBOL_LIT_STRING 404
#define SYMBOL_IDENTIFIER 407


#define SYMBOL_SCALAR 408
#define SYMBOL_FUNCTION 409
#define SYMBOL_VECTOR 410
#define SYMBOL_POINTER 411
#define SYMBOL_LABEL 412


#define DATATYPE_INT 501
#define DATATYPE_CHAR 502
#define DATATYPE_FLOAT 503

#define DATATYPE_INTERNAL_BOOL 504

typedef struct table_entry {
	char *text;
	int type;
	int datatype;
  	struct table_entry *next;
} hash_entry;

hash_entry* hash_table[SIZE];
int hash_generate(void);
int hash_address(char* text);
hash_entry* hash_find(char* text);
hash_entry* hash_insert(int type, char* text);
void hash_print(void);
hash_entry* make_temp();
hash_entry* make_label();
