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
static boolean _analyzeInterpolation(Interpolation* interpolation);
static boolean _analyzeInterpolationFragment(InterpolationFragment* fragment);
static boolean _analyzeIdentifierFactor(Factor* factor);

//--------------------------------------------------------------------------

static void _initializeSemanticAnalyzerModule(SymbolTable* symbolTable) {
	_logger = createLogger("SemanticAnalyzer");
	_symbolTable = symbolTable;
}

static void _shutdownSemanticAnalyzerModule() {
	destroyLogger(_logger);
    _logger = NULL;
    _symbolTable = NULL;
}

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

	if (declaration->type == STRING_TYPE || declaration->type == ATOMIC_TYPE || declaration->type == BUFFER_TYPE) { // Por ahora siempre TRUE
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
		.routine = routine
	};

	if (!defineSymbol(_symbolTable, routine->identifier, &symbol)) {
		logError(_logger, "Failed to define routine: %s", routine->identifier);
		return false;
	}

	return _analyzeStatementList(routine->body);
}

static boolean _analyzeExpression(Expression* expression) {
    _logSemanticAnalizer(__FUNCTION__);

	logDebugging(_logger, "EXPRESSION: %p", expression);
	logDebugging(_logger, "TYPE: %d", expression->type);

	if (!expression) { return false; }

	switch (expression->type) {
		case ARITHMETIC_EXPRESSION:
			return _analyzeExpression(expression->arithmetic->left) &&
				   _analyzeExpression(expression->arithmetic->right);
		case FACTOR_EXPRESSION:
			return _analyzeFactor(expression->factor);
		case EXPRESSION_RND:
			return _analyzeExpression(expression->random->min) &&
			       _analyzeExpression(expression->random->max) &&
			       _analyzeExpression(expression->random->charset);
		case EXPRESSION_REV:
		case EXPRESSION_TUP:
		case EXPRESSION_TLO:
		case EXPRESSION_LEN:
			return _analyzeExpression(expression->unary->input);
		case EXPRESSION_RPL:
			return _analyzeExpression(expression->replace->original) &&
			       _analyzeExpression(expression->replace->target) &&
			       _analyzeExpression(expression->replace->replacement);
		case EXPRESSION_ECP:
			return _analyzeExpression(expression->binary->input) &&
				   _analyzeExpression(expression->binary->key);

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
			return _analyzeInterpolation(factor->interpolation);
        case IDENTIFIER_FACTOR:
            return _analyzeIdentifierFactor(factor);
		default:
			logError(_logger, "Unknown factor type.");
			return false;
	}
}

static boolean _analyzeInterpolation(Interpolation* interpolation) {
    _logSemanticAnalizer(__FUNCTION__);

	if (!interpolation || !interpolation->fragments) { return true; }

	InterpolationFragment* current = interpolation->fragments->head;

	while (current != NULL) {
		if (!_analyzeInterpolationFragment(current)) {
			return false;
		}
        current = current->next;
	}

	return true;
}

static boolean _analyzeInterpolationFragment(InterpolationFragment* fragment) {
    _logSemanticAnalizer(__FUNCTION__);

	if (!fragment) { return false; }

	switch (fragment->type) {
		case LITERAL_FRAGMENT:
			return true;

		case EXPRESSION_FRAGMENT: {
			char* identifier = fragment->identifier;

			if (!isSymbolDefined(_symbolTable, identifier)) {
				logError(_logger, "Undefined identifier in interpolation: '%s'", identifier);
				return false;
			}
            Symbol *symbol = getSymbol(_symbolTable, identifier);
			SymbolKind kind = symbol->kind;

			if (kind != VARIABLE_SYMBOL || symbol->variable.type == BUFFER_TYPE) {
				logWarning(_logger, "This type of interpolating is still not supported '%s'", identifier); // En este caso no considero que deba quedar en error
			}

			return true;
		}

		default:
			logError(_logger, "Unknown interpolation fragment type.");
			return false;
	}
}

static boolean _analyzeIdentifierFactor(Factor* factor) {
    if (!factor || !factor->identifier) { return false; }

    char* identifier = factor->identifier;

    if (!isSymbolDefined(_symbolTable, identifier)) {
        logError(_logger, "Undefined identifier: '%s'", identifier);
        return false;
    }

    Symbol* symbol = getSymbol(_symbolTable, identifier);

    if (symbol->kind != VARIABLE_SYMBOL && symbol->kind != ROUTINE_SYMBOL) {
        logError(_logger, "Identifier '%s' must be a variable or routine.", identifier);
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------

boolean performSemanticAnalysis(Program* program, SymbolTable* symbolTable) {
    _initializeSemanticAnalyzerModule(symbolTable);

    _logSemanticAnalizer(__FUNCTION__);
    boolean status = false;

    if (program == NULL) {
        logError(_logger, "Invalid program.");
    } else if (program->statements == NULL) {
		status = true;
	}
    else {
        status = _analyzeStatementList(program->statements);
    }
    
    _shutdownSemanticAnalyzerModule();

    return status;
}