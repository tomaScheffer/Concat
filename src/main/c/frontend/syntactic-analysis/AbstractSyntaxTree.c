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

// ----------------------------------------------------------------------------------------------

Interpolation* createInterpolation() {
    Interpolation* interpolation = malloc(sizeof(Interpolation));

    if (!interpolation) { return NULL; }

    interpolation->fragments = malloc(sizeof(InterpolationFragmentList));

    if (!interpolation->fragments) {
        free(interpolation);
        return NULL;
    }

    interpolation->fragments->head = NULL;
    interpolation->fragments->next = NULL;

    return interpolation;
}

// ----------------------------------------------------------------------------------------------

void releaseFactor(Factor* factor) {
	if (!factor) { return; }

	switch (factor->type) {
		case CONSTANT_FACTOR:
			free(factor->constant);
			break;
		case EXPRESSION_FACTOR:
			releaseExpression(factor->expression);
			break;
		case INTERPOLATION_FACTOR:
			releaseInterpolation(factor->interpolation);
			break;
	}
	free(factor);
}

void releaseExpression(Expression* expression) {
	if (!expression) return;

	switch (expression->type) {
		case FACTOR_EXPRESSION:
			releaseFactor(expression->factor);
			break;

		case ARITHMETIC_EXPRESSION:
			releaseExpression(expression->arithmetic->left);
			releaseExpression(expression->arithmetic->right);
			break;

		case EXPRESSION_RND:
			releaseExpression(expression->random->min);
			releaseExpression(expression->random->max);
			releaseExpression(expression->random->charset);
			break;

		case EXPRESSION_REV:
		case EXPRESSION_TUP:
		case EXPRESSION_TLO:
			releaseExpression(expression->unary->input);
			break;

		case EXPRESSION_RPL:
			releaseExpression(expression->replace->original);
			releaseExpression(expression->replace->target);
			releaseExpression(expression->replace->replacement);
			break;

		default:
			logWarning(_logger, "releaseExpression: unhandled ExpressionType = %d", expression->type);
			break;
	}

	free(expression);
}


void releaseStatementList(StatementList* list) {
	while (list) {
		StatementList* next = list->next;
		releaseStatement(list->statement);
		free(list);
		list = next;
	}
}

void releaseStatement(Statement* statement) {
	if (!statement) { return; }

	switch (statement->type) {
		case STATEMENT_DECLARATION:
			free(statement->declaration->identifier);
			free(statement->declaration);
			break;
		case STATEMENT_ROUTINE:
			releaseRoutine(statement->routine);
			break;
		case STATEMENT_ROUTINE_CALL:
			free(statement->routineCallName);
			break;
		case STATEMENT_OUTPUT | STATEMENT_EXPRESSION:
			releaseExpression(statement->expression);
			break;
	}
	free(statement);
}

void releaseRoutine(Routine* routine) {
	if (!routine) { return; }

	free(routine->identifier);
	releaseStatementList(routine->body);
	free(routine);
}

void releaseProgram(Program* program) {
	if (!program) { return; }

	releaseStatementList(program->statements);
	free(program);
}

void releaseInterpolationFragmentList(InterpolationFragmentList* list) {
    InterpolationFragment* current = list->head;
	
    while (current) {
        InterpolationFragment* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void releaseInterpolation(Interpolation* interpolation) {
	if (interpolation) {
		releaseInterpolationFragmentList(interpolation->fragments);
		free(interpolation);
	}
}
