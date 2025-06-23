#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../shared/Type.h"

typedef enum {
	TYPE_ATOMIC,
	TYPE_STRING,
	TYPE_BUFFER
} VariableType;

typedef struct {
	const char * name;
	VariableType type;
} Symbol;

typedef struct SymbolTable SymbolTable;

SymbolTable * createSymbolTable();
void destroySymbolTable(SymbolTable * table);
boolean defineSymbol(SymbolTable * table, const char * name, VariableType type);
boolean isSymbolDefined(SymbolTable * table, const char * name);
VariableType getSymbolType(SymbolTable * table, const char * name);
