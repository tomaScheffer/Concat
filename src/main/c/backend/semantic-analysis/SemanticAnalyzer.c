#include "SemanticAnalyzer.h"

static Logger * _logger = NULL;
static SymbolTable * _symbolTable = NULL;

//--------------------------------------------------------------------------

void initializeSemanticAnalyzerModule() {
	_logger = createLogger("SemanticAnalyzer");
	_symbolTable = createSymbolTable();
}

void shutdownSemanticAnalyzerModule() {
	destroySymbolTable(_symbolTable);
	destroyLogger(_logger);
}

//--------------------------------------------------------------------------

boolean analyzeSemantics(Program * program) {
	logDebugging(_logger, "Beginning semantic analysis...");

	boolean response = analyzeStatements(program->statements, _symbolTable);

	if (response) {
		logDebugging(_logger, "Semantic analysis completed successfully.");
	} else {
		logError(_logger, "Semantic analysis failed.");
	}

    return response;
}
