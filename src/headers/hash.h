#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101


/*-------------------------------------------------------------.
| [X] "struct nlist" es el tipo de dato de cada objeto         |
|      en la tabla de la "hash table" o "diccionario"          |
|                                                              |
| [X] "nlist* next" se refiere se refiere al siguiente         |
|      objeto en la cadena de la hash table                    |
|                                                              |
| [X] "char* name" es el nombre para referenciar de cada       |
|      pareja de "key-value" o "nombre-valor" en la tabla      |
|                                                              |
| [X] "char* value" es el valor de cada pareja                 |
`-------------------------------------------------------------*/

struct nlist {
	struct nlist* next;
	char* name;
	char* value;
};

static struct nlist* hashtable[HASHSIZE];   /* tabla de punteros */

/* hash: genera un valor de "hash" para un string str */
unsigned hash(char* str) {
	unsigned hashvalue;

	for (hashvalue = 0; *str != '\0'; str++)
		hashvalue = *str + 31 * hashvalue;
	
	return hashvalue % HASHSIZE;
}

/* lookup: busca el objeto de nombre "str" en hashtable y retorna el objeto */
struct nlist* lookup (char* str) {
	struct nlist* entry;

	for (entry = hashtable[hash(str)]; entry != NULL; entry = entry->next)
		if(strcmp(str, entry->name) == 0)
			return entry;
	return NULL;
}

/* install: agrega una entrada con la pareja (name, value) a hashtable */
struct nlist* install(char* name, char* value) {
	struct nlist* entry = lookup(name);
	unsigned hashvalue;

	if (entry == NULL) {	/* nombre de entrada es único (no existía) */
		entry = (struct nlist*) malloc(sizeof(*entry));

		if (entry == NULL || (entry->name = strdup(name)) == NULL)
			return NULL;
		
		hashvalue = hash(name);
		entry->next = hashtable[hashvalue];
		hashtable[hashvalue] = entry;
	} else		/* nombre de entrada ya existía */
		free((void*) entry->value);	/* liberar datos anteriores */

	if ((entry->value = strdup(value)) == NULL)
		return NULL;

	return entry;
}