#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../semantic-analysis/SymbolTable.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Generates the final output using the current compiler state.
 */
void generate(CompilerState* compilerState, SymbolTable* symbolTable);

#endif
