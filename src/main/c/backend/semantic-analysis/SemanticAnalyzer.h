#ifndef SEMANTICANALIZER_HEADER
#define SEMANTICANALIZER_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "SymbolTable.h"

// ------------------------------------------------------------------------------------

boolean performSemanticAnalysis(Program* program, SymbolTable* symbolTable);

#endif