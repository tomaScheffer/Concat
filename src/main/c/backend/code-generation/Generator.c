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

static char* _evaluateRandom(RandomExpression* random);
static char* _evaluateReverse(UnaryExpression* unary);
static char* _evaluateToUpper(UnaryExpression* unary);
static char* _evaluateToLower(UnaryExpression* unary);
static char* _evaluateLenght(UnaryExpression* unary);
static char* _evaluateReplace(ReplaceExpression* replace);
static char* _evaluateSimpleEncription(BinaryExpression* binary);

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
            VariableData data;
			data.type = statement->declaration->type;

			switch (statement->declaration->type) {
				case STRING_TYPE: {
					char* value = _evaluateExpression(statement->declaration->expression);
					data.stringValue = _duplicateString(value);
					free(value);
					break;
				}
				case ATOMIC_TYPE: {
					int value = statement->declaration->atomicValue;
					data.atomicValue = value;
					break;
				}
				default:
					logWarning(_logger, "Unhandled declaration type");
					break;
			}

			setSymbolValue(_symbolTable, statement->declaration->identifier, data);
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
		case ARITHMETIC_EXPRESSION: // TODO!
			return strdup("ARITHMETIC_EXPRESSION");
        case EXPRESSION_RND:
            return _evaluateRandom(expression->random);
        case EXPRESSION_REV:
            return _evaluateReverse(expression->unary);
        case EXPRESSION_TUP:
            return _evaluateToUpper(expression->unary);
        case EXPRESSION_TLO:
            return _evaluateToLower(expression->unary);
        case EXPRESSION_LEN:
            return _evaluateLenght(expression->unary);
        case EXPRESSION_RPL:
            return _evaluateReplace(expression->replace);
        case EXPRESSION_ECP:
            return _evaluateSimpleEncription(expression->binary);
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
        case EXPRESSION_FACTOR:
            return _evaluateExpression(factor->expression);
        case CONSTANT_FACTOR: {
            if (factor->constant->type == ATOMIC_TYPE) {
                char buffer[32];
                snprintf(buffer, sizeof(buffer), "%d", factor->constant->atomic);
                return strdup(buffer);
            } else if (factor->constant->type == STRING_TYPE) {
                return strdup(factor->constant->string);
            }
        }
		case IDENTIFIER_FACTOR: {
			char* identifier = factor->identifier;
			Symbol* symbol = getSymbol(_symbolTable, identifier);

			if (!symbol) {
				logError(_logger, "Undefined identifier: '%s'", identifier);
				return _duplicateString("");
			}

			if (symbol->kind == VARIABLE_SYMBOL) {
                switch (symbol->variable.type) {
                    case STRING_TYPE:
                        return strdup(symbol->variable.stringValue);
                    case ATOMIC_TYPE: {
                        char buffer[32];
                        snprintf(buffer, sizeof(buffer), "%d", symbol->variable.atomicValue);
                        return strdup(buffer);
                    }

                    default:
                        logWarning(_logger, "Unsupported type in interpolation for '%s'", identifier);
                        return strdup("<?>");
			    }
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

    while (current) { // Appendear fragmentos
        InterpolationFragment* fragment = current->head;
        char* value = NULL;

        if (fragment->type == LITERAL_FRAGMENT) {
            value = _duplicateString(fragment->literal);

        } else if (fragment->type == EXPRESSION_FRAGMENT) {

            char* identifier = fragment->identifier;
            Symbol* symbol = getSymbol(_symbolTable, identifier);

            if (!symbol || symbol->kind != VARIABLE_SYMBOL) {
                logError(_logger, "Undefined or non-variable identifier in interpolation: '%s'", identifier);
                value = _duplicateString("<?>");
            } else {
                if (symbol->variable.type == STRING_TYPE && symbol->variable.stringValue) {
                    value = _duplicateString(symbol->variable.stringValue);
                } else if (symbol->variable.type == ATOMIC_TYPE) {
                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "%d", symbol->variable.atomicValue);
                    value = _duplicateString(buffer);
                }
                else {
                    value = _duplicateString("<?>");
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

static char* _evaluateRandom(RandomExpression* random) {
    char* minStr = _evaluateExpression(random->min);
    char* maxStr = _evaluateExpression(random->max);
    char* charset = _evaluateExpression(random->charset);

    if (!minStr || !maxStr || !charset) {
        free(minStr);
		free(maxStr);
		free(charset);
        return NULL;
    }

    int min = atoi(minStr);
    int max = atoi(maxStr);
    free(minStr);
    free(maxStr);

    if (min > max || max <= 0 || min < 0) {
        free(charset);
        return NULL;
    }

    int length = min + (rand() % (max - min + 1));
    int charsetLen = strlen(charset);
    char* result = malloc(length + 1);

    if (!result) {
        free(charset);
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        result[i] = charset[rand() % charsetLen];
    }

    result[length] = '\0';
    free(charset);

    return result;
}

static char* _evaluateReverse(UnaryExpression* unary) {
    char* input = _evaluateExpression(unary->input);

    if (!input) { return NULL; }

    int lenght = strlen(input);
    char* result = malloc(lenght + 1);

    if (!result) {
        free(input);
        return NULL;
    }

    for (int i = 0; i < lenght; i++) {
        result[i] = input[lenght - 1 - i];
    }

    result[lenght] = '\0';
    free(input);

    return result;
}

static char* _evaluateToUpper(UnaryExpression* unary) {
    char* input = _evaluateExpression(unary->input);

    if (!input) { return NULL; }

    for (char* character = input; *character; character++) {
		*character = toupper(*character);
	}

    return input;
}

static char* _evaluateToLower(UnaryExpression* unary) {
    char* input = _evaluateExpression(unary->input);

    if (!input) { return NULL; }

    for (char* character = input; *character; character++) {
		*character = tolower(*character);
	}

    return input;
}

static char* _evaluateLenght(UnaryExpression* unary) {
    char* input = _evaluateExpression(unary->input);

    if (!input) { return NULL; }

    int lenght = strlen(input);
    char* result = malloc(16);
    sprintf(result, "%d", lenght);
    free(input);

    return result;
}

static char* _evaluateReplace(ReplaceExpression* replace) {
    char* original = _evaluateExpression(replace->original);
    char* target = _evaluateExpression(replace->target);
    char* replacement = _evaluateExpression(replace->replacement);

    if (!original || !target || !replacement) {
        free(original);
		free(target);
		free(replacement);
        return NULL;
    }

    int count = 0;
    char* pos = original;
    size_t targetLenght = strlen(target);
    size_t replacementLenght = strlen(replacement);
	size_t originalLenght;
	size_t newLenght;
	char* result;
	char* destiny;
	char* source;

    while ((pos = strstr(pos, target)) != NULL) {
        count++;
        pos += targetLenght;
    }

    originalLenght = strlen(original);
    newLenght = originalLenght + count * (replacementLenght - targetLenght);
    result = malloc(newLenght + 1);

    if (!result) {
        free(original);
		free(target);
		free(replacement);
        return NULL;
    }

    destiny = result;
    source = original;

    while ((pos = strstr(source, target)) != NULL) {
        size_t bytes = pos - source;

        memcpy(destiny, source, bytes);
        destiny += bytes;
        memcpy(destiny, replacement, replacementLenght);
        destiny += replacementLenght;
        source = pos + targetLenght;
    }

    strcpy(destiny, source);

    free(original);
    free(target);
    free(replacement);

    return result;
}

static char* _evaluateSimpleEncription(BinaryExpression* binary) {
    char* input = _evaluateExpression(binary->input);
    char* key = _evaluateExpression(binary->key);

    if (!input || !key) {
        free(input);
        free(key);
        return _duplicateString("");
    }

    size_t len = strlen(input);
    size_t keyLenght = strlen(key);
    size_t minLenght = len < keyLenght ? len : keyLenght;

    unsigned char* xorResult = malloc(minLenght);

    if (!xorResult) {
        free(input);
        free(key);
        return _duplicateString("");
    }

    for (size_t i = 0; i < minLenght; ++i) {
        xorResult[i] = input[i] ^ key[i % keyLenght];
    }

    size_t encodedLenght = 0;
    char* encoded = base64_encode(xorResult, minLenght, &encodedLenght);

    free(xorResult);
    free(input);
    free(key);

    if (!encoded) { return _duplicateString(""); }

    return encoded;
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
    _output(0, "%s", "");
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
