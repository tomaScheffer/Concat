#ifndef SYMBOLTABLE_HEADER
#define SYMBOLTABLE_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Type.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
	TYPE_ATOMIC,
	TYPE_STRING,
	TYPE_BUFFER
} VariableType;

typedef enum {
	VARIABLE_SYMBOL,
	ROUTINE_SYMBOL
} SymbolKind;

typedef struct {
    char* name;
	SymbolKind kind;

	union {
		struct {
			VariableType type;
		} variable;

		struct {
			Routine* routine;
		} routine;
	};
} Symbol;

typedef struct SymbolTable SymbolTable;

SymbolTable* createSymbolTable();
void destroySymbolTable(SymbolTable* table);

boolean defineVariableSymbol(SymbolTable* table, char* name, Symbol symbol);
boolean isSymbolDefined(SymbolTable* table, char* name);
Symbol* getSymbol(SymbolTable* table, char* name);

#endif