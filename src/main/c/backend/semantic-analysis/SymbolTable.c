#include "SymbolTable.h"

#define INITIAL_CAPACITY        64

struct SymbolTable {
    Symbol* entries;
    int size;
    int capacity;
};


static void expand(SymbolTable * table) {
    table->capacity *= 2;
    table->entries = realloc(table->entries, sizeof(Symbol) * table->capacity);
}

SymbolTable* createSymbolTable() {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->entries = malloc(sizeof(Symbol) * INITIAL_CAPACITY);
    table->size = 0;
    table->capacity = INITIAL_CAPACITY;

    return table;
}

void destroySymbolTable(SymbolTable * table) {
    if (!table) { return; }

    for (int i = 0; i < table->size; i++) {
        free((char *)table->entries[i].name);
    }
    free(table->entries);
    free(table);
}

boolean defineSymbol(SymbolTable * table, const char * name, VariableType type) {
    if (isSymbolDefined(table, name)) {
        return false;
    }
    if (table->size == table->capacity) {
        expand(table);
    }
    table->entries[table->size].name = strdup(name);
    table->entries[table->size].type = type;
    table->size++;

    return true;
}

boolean isSymbolDefined(SymbolTable * table, const char * name) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->entries[i].name, name) == 0) {
            return true;
        }
    }

    return false;
}

VariableType getSymbolType(SymbolTable * table, const char * name) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->entries[i].name, name) == 0) {
            return table->entries[i].type;
        }
    }

    return -1;
}