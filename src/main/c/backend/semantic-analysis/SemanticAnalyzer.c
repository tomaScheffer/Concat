#include "SemanticAnalyzer.h"

static Logger* _logger = NULL;
static SymbolTable* _symbolTable = NULL;

//--------------------------------------------------------------------------

static boolean _analyzeStatementList(StatementList* list);
static boolean _analyzeStatement(Statement* Statement);
static boolean _analyzeDeclaration(Declaration* declaration);
static boolean _analyzeExpression(Expression* expression);
static boolean _analyzeFactor(Factor* factor);
static boolean _analyzeRoutine(Routine* routine);

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

static void _logSemanticAnalizer(const char* functionName) {
	logDebugging(_logger, "%s", functionName);
}

static boolean _analyzeStatementList(StatementList* list) {
	if (list == NULL) { return true; }

	return _analyzeStatementList(list->next) && _analyzeStatement(list->statement);
}

static boolean _analyzeStatement(Statement* statement) {
    _logSemanticAnalizer(__FUNCTION__);

	if (statement == NULL) { return false; }

	switch (statement->type) {
		case STATEMENT_DECLARATION:
			return _analyzeDeclaration(statement->declaration);
		case STATEMENT_EXPRESSION:
			return _analyzeExpression(statement->expression);
		case STATEMENT_ROUTINE:
			return _analyzeRoutine(statement->routine);
		case STATEMENT_ROUTINE_CALL: {
			if (!isSymbolDefined(_symbolTable, statement->routineCallName)) {
				logError(_logger, "Undefined routine: %s", statement->routineCallName);
				return false;
			}
			return true;
		}
		case STATEMENT_STRING_OPERATION:
			// TODO: revisar x si requiere identificadores
			return true;
		case STATEMENT_OUTPUT:
			return _analyzeExpression(statement->expression);
		default:
			logError(_logger, "Unknown statement type.");
			return false;
	}
}

static boolean _analyzeDeclaration(Declaration* declaration) {
    _logSemanticAnalizer(__FUNCTION__);

	if (!declaration || !declaration->identifier) { return false; }

	if (isSymbolDefined(_symbolTable, declaration->identifier)) {
		logError(_logger, "Variable '%s' already declared", declaration->identifier);
		return false;
	}

	Symbol symbol = {
		.kind = VARIABLE_SYMBOL,
		.variable.type = declaration->type
	};

	if (!defineSymbol(_symbolTable, declaration->identifier, &symbol)) {
		logError(_logger, "Failed to define symbol for: %s", declaration->identifier);
		return false;
	}

	if (declaration->type == TYPE_STRING || declaration->type == TYPE_BUFFER) {
		return true;
	} else {
		return _analyzeExpression(declaration->expression);
	}
}

static boolean _analyzeRoutine(Routine* routine) {
    _logSemanticAnalizer(__FUNCTION__);

	if (!routine || !routine->identifier) { return false; }

	if (isSymbolDefined(_symbolTable, routine->identifier)) {
		logError(_logger, "Routine '%s' already defined", routine->identifier);

		return false;
	}

	Symbol symbol = {
		.kind = ROUTINE_SYMBOL,
		.routine.routine = routine
	};

	if (!defineSymbol(_symbolTable, routine->identifier, &symbol)) {
		logError(_logger, "Failed to define routine: %s", routine->identifier);
		return false;
	}

	return _analyzeStatementList(routine->body);
}

static boolean _analyzeExpression(Expression* expression) {
    _logSemanticAnalizer(__FUNCTION__);

	if (!expression) { return false; }

	switch (expression->type) {
		case ARITHMETIC_EXPRESSION:
			return _analyzeExpression(expression->leftExpression) &&
				   _analyzeExpression(expression->rightExpression);
		case FACTOR_EXPRESSION:
			return _analyzeFactor(expression->factor);
		default:
			logError(_logger, "Unknown expression type.");
			return false;
	}
}

static boolean _analyzeFactor(Factor* factor) {
    _logSemanticAnalizer(__FUNCTION__);

	if (!factor) { return false; }

	switch (factor->type) {
		case CONSTANT_FACTOR:
			return true;
		case EXPRESSION_FACTOR:
			return _analyzeExpression(factor->expression);
		case INTERPOLATION_FACTOR:
			// TODO
			return true;
		default:
			logError(_logger, "Unknown factor type.");
			return false;
	}
}

//--------------------------------------------------------------------------

boolean performSemanticAnalysis(Program* program) {
    _logSemanticAnalizer(__FUNCTION__);

    if (program == NULL || program->statements == NULL) {
        logError(_logger, "Invalid program.");
        return false;
    }

    SymbolTable* table = createSymbolTable();
    boolean status = analyzeStatementList(program->statements, table);
    destroySymbolTable(table);
    
    return status;
}