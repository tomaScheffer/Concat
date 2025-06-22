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
	Interpolation* interp = malloc(sizeof(Interpolation));
	interp->fragments = NULL;

	return interp;
}

void addInterpolationFragment(Interpolation* interpolation, InterpolationFragment* fragment) {
	if (!interpolation->fragments) {
		interpolation->fragments = fragment;
	} else {
		InterpolationFragment* current = interpolation->fragments;
		while (current->next) {
			current = current->next;
		}
		current->next = fragment;
	}
}

Interpolation* InterpolationSemanticAction(InterpolationFragment* list) {
    Interpolation* interpolation = createInterpolation();
    InterpolationFragment* current = list;

    while (current != NULL) {
        InterpolationFragment* next = current->next;
        current->next = NULL; // Desvincular antes de agregar
        addInterpolationFragment(interpolation, current);
        current = next;
    }

    return interpolation;
}

// ----------------------------------------------------------------------------------------------

static void releaseInterpolationFragments(InterpolationFragment* fragment) {
	while (fragment) {
		InterpolationFragment* next = fragment->next;
		if (fragment->type == LITERAL_FRAGMENT) {
			free(fragment->literal);
		} else if (fragment->type == EXPRESSION_FRAGMENT) {
			free(fragment->identifier);
		}
		free(fragment);
		fragment = next;
	}
}

void releaseInterpolation(Interpolation* interpolation) {
	if (interpolation) {
		releaseInterpolationFragments(interpolation->fragments);
		free(interpolation);
	}
}

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

void releaseStatement(Statement* stmt) {
	if (!stmt) { return; }
	switch (stmt->type) {
		case STATEMENT_DECLARATION:
			free(stmt->declaration->identifier);
			free(stmt->declaration);
			break;
		case STATEMENT_ROUTINE:
			releaseRoutine(stmt->routine);
			break;
		case STATEMENT_ROUTINE_CALL:
			free(stmt->routineCallName);
			break;
		case STATEMENT_OUTPUT:
			releaseExpression(stmt->outputExpression);
			break;
	}
	free(stmt);
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
