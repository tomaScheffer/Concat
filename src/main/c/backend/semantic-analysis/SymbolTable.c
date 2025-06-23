#include "SymbolTable.h"

#define INITIAL_CAPACITY        64

static Logger* _logger = NULL;

struct SymbolTable {
    Symbol* entries;
    int size;
    int capacity;
};

//--------------------------------------------------------------------------------

static void expand(SymbolTable* table) {
    table->capacity *= 2;
    table->entries = realloc(table->entries, sizeof(Symbol) * table->capacity);
    // Acá ya asumo que funca siempre (TODO: validar realloc)
}

static void _logSymbolTable(const char* functionName) {
	logDebugging(_logger, "%s", functionName);
}

//--------------------------------------------------------------------------------

SymbolTable* createSymbolTable() {
    _logSymbolTable(__FUNCTION__);

    SymbolTable* table = malloc(sizeof(SymbolTable));

    if (!table) { return NULL; }
    table->entries = malloc(sizeof(Symbol) * INITIAL_CAPACITY);

    if (!table->entries) {
        free (table);
        return NULL;
    }
    table->size = 0;
    table->capacity = INITIAL_CAPACITY;

    return table;
}

void destroySymbolTable(SymbolTable* table) {
    _logSymbolTable(__FUNCTION__);

    if (!table) { return; }

    for (int i = 0; i < table->size; i++) {
        free((char *)table->entries[i].name);
    }

    free(table->entries);
    free(table);
}

//--------------------------------------------------------------------------------

boolean defineSymbol(SymbolTable* table, char* name, Symbol symbol) {
    _logSymbolTable(__FUNCTION__);

    if (isSymbolDefined(table, name)) { return false; }

    if (table->size >= table->capacity) {
        expand(table);
    }

    char* copiedName = strdup(name);
    if (!copiedName) { return false; }

    symbol.name = copiedName;
    table->entries[table->size++] = symbol;

    return true;
}

boolean isSymbolDefined(SymbolTable* table, char* name) {
    _logSymbolTable(__FUNCTION__);

    for (int i = 0; i < table->size; ++i) {
        if (strcmp(table->entries[i].name, name) == 0) {
            return true;
        }
    }

    return false;
}

Symbol* getSymbol(SymbolTable* table, char* name) {
    _logSymbolTable(__FUNCTION__);
    
    for (int i = 0; i < table->size; ++i) {
        if (strcmp(table->entries[i].name, name) == 0) {
            return &table->entries[i];
        }
    }

    return NULL;
}