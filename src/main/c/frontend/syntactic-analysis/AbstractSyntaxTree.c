#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */
static Logger* _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseConstant(Constant* constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseExpression(Expression* expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
		case ADDITION:
		case DIVISION:
		case MULTIPLICATION:
		case SUBTRACTION:
			releaseExpression(expression->leftExpression);
			releaseExpression(expression->rightExpression);
			break;
		case FACTOR:
			releaseFactor(expression->factor);
			break;
		}
		free(expression);
	}
}

void releaseFactor(Factor* factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
		case CONSTANT:
			releaseConstant(factor->constant);
			break;
		case EXPRESSION:
			releaseExpression(factor->expression);
			break;
		}
		free(factor);
	}
}

void releaseStringOperation(StringOperation* operation) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (operation != NULL) {
		free(operation->arg1);
		free(operation->arg2);
		free(operation->arg3);
		free(operation);
	}
}

void releaseDeclaration(Declaration* declaration) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (declaration != NULL) {
		free(declaration->identifier);
		switch (declaration->isString) {
		case 0:
			releaseExpression(declaration->expression);
			break;
		case 1:
			free(declaration->stringLiteral);
			break;
		case 2:
			releaseStringOperation(declaration->stringOperation);
			break;
		}
		free(declaration);
	}
}

void releaseDeclarationList(DeclarationList* list) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	while (list != NULL) {
		DeclarationList* next = list->next;
		releaseDeclaration(list->declaration);
		free(list);
		list = next;
	}
}

void releaseProgram(Program* program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseDeclarationList(program->declarations);
		free(program);
	}
}
