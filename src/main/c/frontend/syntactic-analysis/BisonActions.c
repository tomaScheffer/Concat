#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Program* ProgramSemanticAction(DeclarationList* declarations) {
	return createProgram(declarations);
}

DeclarationList* AppendDeclarationListSemanticAction(DeclarationList* list, Declaration* declaration) {
	return appendDeclaration(list, declaration);
}

DeclarationList* EmptyDeclarationListSemanticAction() {
	return createDeclarationList();
}

Declaration* StringDeclarationSemanticAction(char* identifier, Expression* expression) {
	return createDeclaration(STRING_TYPE, identifier, expression);
}

Declaration* AtomicDeclarationSemanticAction(char* identifier, Expression* expression) {
	return createDeclaration(ATOMIC_TYPE, identifier, expression);
}

Declaration* BufferDeclarationSemanticAction(char* identifier, Expression* expression) {
	return createDeclaration(BUFFER_TYPE, identifier, expression);
}

Expression* StringOperationExpressionSemanticAction(StringOperation* operation) {
	return createStringOperationExpression(operation);
}

Expression* IdentifierExpressionSemanticAction(char* identifier) {
	return createIdentifierExpression(identifier);
}

Expression* StringLiteralExpressionSemanticAction(char* literal) {
	return createStringLiteralExpression(literal);
}

Expression* IntegerLiteralExpressionSemanticAction(int value) {
	return createIntegerLiteralExpression(value);
}

Expression* InputExpressionSemanticAction() {
	return createInputExpression();
}

StringOperation* RndOperationSemanticAction(Expression* min, Expression* max, Expression* charset) {
	return createRndOperation(min, max, charset);
}

StringOperation* RevOperationSemanticAction(Expression* input) {
	return createRevOperation(input);
}

StringOperation* TupOperationSemanticAction(Expression* input) {
	return createTupOperation(input);
}

StringOperation* TloOperationSemanticAction(Expression* input) {
	return createTloOperation(input);
}

StringOperation* RplOperationSemanticAction(Expression* original, Expression* from, Expression* to) {
	return createRplOperation(original, from, to);
}

StringOperation* NumOperationSemanticAction(Expression* input) {
	return createNumOperation(input);
}

StringOperation* LenOperationSemanticAction(Expression* input) {
	return createLenOperation(input);
}

StringOperation* EncOperationSemanticAction(char* method, Expression* key, Expression* data) {
	return createEncOperation(method, key, data);
}

StringOperation* DecOperationSemanticAction(char* method, Expression* key, Expression* encrypted) {
	return createDecOperation(method, key, encrypted);
}
