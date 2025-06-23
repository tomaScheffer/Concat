#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"

typedef enum {
	TYPE_STRING,
	TYPE_ATOMIC,
	TYPE_BUFFER
} VariableType;

typedef struct {
	char* identifier;
	VariableType type;
	boolean isInitialized;
} Symbol;

typedef struct SymbolTable {
	Symbol** symbols;
	unsigned int count;
	unsigned int capacity;
} SymbolTable;

// ------------------------------------------------------

void initializeSemanticAnalyzerModule();
void shutdownSemanticAnalyzerModule();
int runSemanticAnalysis(Program* program);

boolean analyzeStatements(StatementList * list, SymbolTable * table);
boolean analyzeStatement(Statement * stmt, SymbolTable * table);
boolean analyzeExpression(Expression * expr, SymbolTable * table);
boolean analyzeInterpolation(Interpolation * interpolation, SymbolTable * table);
