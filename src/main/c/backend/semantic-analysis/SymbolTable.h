#ifndef SYMBOLTABLE_HEADER
#define SYMBOLTABLE_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Type.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
	VARIABLE_SYMBOL,
	ROUTINE_SYMBOL
} SymbolKind;

typedef struct {
	ConstantType type;
	char * value;
} VariableData;

typedef struct {
    char* name;
	SymbolKind kind;

	union {
		VariableData variable;
	    Routine* routine;
	};
} Symbol;

typedef struct SymbolTable SymbolTable;

SymbolTable* createSymbolTable();
void destroySymbol(Symbol* symbol);
void destroySymbolTable(SymbolTable* table);

boolean defineVariableSymbol(SymbolTable* table, char* name, Symbol symbol);
boolean isSymbolDefined(SymbolTable* table, char* name);
Symbol* getSymbol(SymbolTable* table, char* name);
boolean setSymbolValue(SymbolTable* table, char* name, VariableData value);

#endif