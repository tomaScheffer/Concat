#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger* _logger = NULL;
static SymbolTable* _symbolTable = NULL;

//------------------------------------------------------------------------------------------------------

/** PRIVATE FUNCTIONS */

static boolean _executeStatementList(StatementList* list);
static boolean _executeStatement(Statement* statement);
static char* _evaluateExpression(Expression* expression);
static char* _evaluateFactor(Factor* factor);
static char* _evaluateInterpolation(Interpolation* interpolation);
static char* _duplicateString(char* text);
static void _executeRoutine(char* identifier);

static void _generateProgram(Program* program);
static void _generatePrologue(void);
static void _generateEpilogue(const int value);
static char* _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char* const format, ...);

//------------------------------------------------------------------------------------------------------

static void _initializeGeneratorModule(SymbolTable* symbolTable) {
	_logger = createLogger("Generator");
	_symbolTable = symbolTable;
}

static void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
	_symbolTable = NULL;
}

static void _logGenerator(const char* functionName) {
	logDebugging(_logger, "%s", functionName);
}

static boolean _executeStatementList(StatementList* list) {
	_logGenerator(__FUNCTION__);

	if (!list) { return true; }

	return _executeStatementList(list->next) && _executeStatement(list->statement);
}

static boolean _executeStatement(Statement* statement) {
	_logGenerator(__FUNCTION__);

	if (!statement) { return false; }

	switch (statement->type) {
		case STATEMENT_DECLARATION: {
			Symbol symbol = {
				.kind = VARIABLE_SYMBOL,
				.variable.type = statement->declaration->type
			};

			defineSymbol(_symbolTable, statement->declaration->identifier, &symbol);

			if (statement->declaration->type == STRING_TYPE) {
				char* value = _evaluateExpression(statement->declaration->expression);
				VariableData data;
				data.type = STRING_TYPE;
				data.value = _duplicateString(value);

				setSymbolValue(_symbolTable, statement->declaration->identifier, data);
				free(value);
			}
			return true;
		}
		case STATEMENT_OUTPUT: {
			char* result = _evaluateExpression(statement->expression);

			_output(0, "%s\n", result);
			free(result);

			return true;
		}
		case STATEMENT_ROUTINE:
			return true;
		case STATEMENT_ROUTINE_CALL:
			_executeRoutine(statement->routineCallName);

			return true;
		default:
			logWarning(_logger, "Unsupported statement type for execution.");
			return true;
	}
}

static char* _evaluateExpression(Expression* expression) {
	_logGenerator(__FUNCTION__);

	if (!expression) { return _duplicateString(""); }

	switch (expression->type) {
		case FACTOR_EXPRESSION:
			return _evaluateFactor(expression->factor);
		case ARITHMETIC_EXPRESSION: // TODO
		default:
			logError(_logger, "Unsupported expression type.");
			return _duplicateString("");
	}
}

static char* _evaluateFactor(Factor* factor) {
	_logGenerator(__FUNCTION__);

	if (!factor) { return _duplicateString(""); }

	switch (factor->type) {
		case INTERPOLATION_FACTOR:
			return _evaluateInterpolation(factor->interpolation);
		case CONSTANT_FACTOR:
			return _duplicateString(factor->constant->string);
		case EXPRESSION_FACTOR:
			return _evaluateExpression(factor->expression);
		case IDENTIFIER_FACTOR: {
			const char* identifier = factor->identifier;
			Symbol* symbol = getSymbol(_symbolTable, (char*) identifier);

			if (!symbol) {
				logError(_logger, "Undefined identifier: '%s'", identifier);
				return _duplicateString("");
			}

			if (symbol->kind == VARIABLE_SYMBOL) {
				return _duplicateString(symbol->variable.value ? symbol->variable.value : "");
			} else if (symbol->kind == ROUTINE_SYMBOL) {
				_executeRoutine(symbol->routine->identifier);
				return _duplicateString("");
			} else {
				logError(_logger, "Identifier '%s' is not a valid variable or routine.", identifier);
				return _duplicateString("");
			}
		}
		default:
			logError(_logger, "Unsupported factor type.");
			return _duplicateString("");
	}
}

static char* _evaluateInterpolation(Interpolation* interpolation) {
	_logGenerator(__FUNCTION__);

    if (!interpolation || !interpolation->fragments) { 
        return _duplicateString(""); 
    }

    InterpolationFragmentList* current = interpolation->fragments;
    char* result = _duplicateString("");

    while (current) { // Appendear
        InterpolationFragment* fragment = current->head;
        char* value = NULL;

        if (fragment->type == LITERAL_FRAGMENT) {
            value = _duplicateString(fragment->literal);

        } else if (fragment->type == EXPRESSION_FRAGMENT) {

            const char* identifier = fragment->identifier;
            Symbol* symbol = getSymbol(_symbolTable, (char*) identifier);

            if (!symbol || symbol->kind != VARIABLE_SYMBOL) {
                logError(_logger, "Undefined or non-variable identifier in interpolation: '%s'", identifier);
                value = _duplicateString("");
            } else {
                if (symbol->variable.value) {
                    value = _duplicateString(symbol->variable.value);
                } else {
                    value = _duplicateString("");
                }
            }

        } else {
            logError(_logger, "Unknown interpolation fragment.");
            value = _duplicateString("");
        }

        char* newResult = malloc(strlen(result) + strlen(value) + 1);

        if (!newResult) {
            logError(_logger, "Out of memory during interpolation evaluation");
            free(result);
            free(value);

            return NULL;
        }
        strcpy(newResult, value);
        strcat(newResult, result);

        free(result);
        free(value);

        result = newResult;
        current = current->next;
    }

    return result;
}

static char* _duplicateString(char* text) {
	_logGenerator(__FUNCTION__);

	if (!text) { return NULL; }

	char* copy = malloc(strlen(text) + 1);
	strcpy(copy, text);

	return copy;
}

static void _executeRoutine(char* identifier) {
	_logGenerator(__FUNCTION__);

	if (!identifier) { return; }

	Symbol* symbol = getSymbol(_symbolTable, (char*) identifier);

	if (!symbol) {
		logError(_logger, "Routine '%s' is not defined", identifier);
		return;
	}

	if (symbol->kind != ROUTINE_SYMBOL || !symbol->routine) {
		logError(_logger, "Symbol '%s' is not a routine", identifier);
		return;
	}

	logDebugging(_logger, "Executing routine: %s", identifier);
	_executeStatementList(symbol->routine->body);
}

//------------------------------------------------------------------------------------------------------

/**
Generates the output of the program.
 */
static void _generateProgram(Program* program) {
	_executeStatementList(program->statements);
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	_output(0, "%s", "");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(const int value) {
	_output(0, "%s%d%s", "");
}

/**
 * Generates an indentation string for the specified level.
 */
static char* _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char* const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char* indentation = _indentation(indentationLevel);
	char* effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

//------------------------------------------------------------------------------------------------------

/** PUBLIC FUNCTIONS */

void generate(CompilerState* compilerState, SymbolTable* symbolTable) {
	_initializeGeneratorModule(symbolTable);
	
	logDebugging(_logger, "Generating final output...");
	
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	
	logDebugging(_logger, "Generation is done.");

	_shutdownGeneratorModule();
}
