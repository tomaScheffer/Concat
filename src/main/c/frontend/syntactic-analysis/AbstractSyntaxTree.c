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

static InterpolationFragment* createInterpolationFragmentLiteral(const char* text) {
	InterpolationFragment* fragment = malloc(sizeof(InterpolationFragment));
	fragment->type = LITERAL_FRAGMENT;
	fragment->literal = strdup(text);
	fragment->next = NULL;
	
	return fragment;
}

static InterpolationFragment* createInterpolationFragmentExpression(const char* identifier) {
	InterpolationFragment* fragment = malloc(sizeof(InterpolationFragment));
	fragment->type = EXPRESSION_FRAGMENT;
	fragment->identifier = strdup(identifier);
	fragment->next = NULL;

	return fragment;
}

Interpolation* createInterpolation() {
    Interpolation* interpolation = malloc(sizeof(Interpolation));

    if (!interpolation) { return NULL; }

    interpolation->fragments = malloc(sizeof(InterpolationFragmentList));

    if (!interpolation->fragments) {
        free(interpolation);
        return NULL;
    }

    interpolation->fragments->head = NULL;
    interpolation->fragments->tail = NULL;

    return interpolation;
}

void addInterpolationFragment(InterpolationFragmentList* list, InterpolationFragment* fragment) {
    if (!list->head) {
        list->head = fragment;
        list->tail = fragment;
    } else {
        list->tail->next = fragment;
        list->tail = fragment;
    }
    fragment->next = NULL;
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

void releaseExpression(Expression* expr) {
	if (!expr) { return; }

	if (expr->type == FACTOR_EXPRESSION) {
		releaseFactor(expr->factor);
	} else {
		releaseExpression(expr->leftExpression);
		releaseExpression(expr->rightExpression);
	}
	free(expr);
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
