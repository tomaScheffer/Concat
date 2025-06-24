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

Program* ProgramSemanticAction(CompilerState *compilerState, StatementList* statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Program* program = malloc(sizeof(Program));
    if (!program) { return NULL; }

    program->statements = statements;

    compilerState->abstractSyntaxtTree = program;
    compilerState->succeed = true;

    return program;
}

StatementList* StatementListSemanticAction(StatementList* next, Statement* statement) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    StatementList* list = malloc(sizeof(StatementList));
    if (!list) { return NULL; }

    list->statement = statement;
    list->next = next;

    return list;
}

// ---------------------------------------------------------------------------------------

Statement* RoutineStatementSemanticAction(Routine* routine) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Statement* statement = malloc(sizeof(Statement));
    if (!statement) { return NULL; }

    statement->type = STATEMENT_ROUTINE;
    statement->routine = routine;

    return statement;
}


Statement* RoutineCallStatementSemanticAction(char* identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Statement* statement = malloc(sizeof(Statement));
    if (!statement) { return NULL; }

    statement->type = STATEMENT_ROUTINE_CALL;
    statement->routineCallName = identifier;

    return statement;
}

Statement* OutStatementSemanticAction(Expression* expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Statement* statement = malloc(sizeof(Statement));
    if (!statement) { return NULL; }

    statement->type = STATEMENT_OUTPUT;
    statement->expression = expression;

    return statement;
}

Statement* DeclarationStatementSemanticAction(Declaration* declaration) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    Statement* statement = malloc(sizeof(Statement));
    if (!statement) { return NULL; }

    statement->type = STATEMENT_DECLARATION;
    statement->declaration = declaration;

    return statement;
}

Statement* ExpressionStatementSemanticAction(Expression* expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    Statement* statement = malloc(sizeof(Statement));
    if (!statement) { return NULL; }

    statement->type = STATEMENT_EXPRESSION;
    statement->expression = expression;

    return statement;
}

Routine* RoutineSemanticAction(char* identifier, StatementList* body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Routine* routine = malloc(sizeof(Routine));

    if (!routine) { return NULL; }

    routine->identifier = identifier;
    routine->body = body;

    return routine;
}

// ---------------------------------------------------------------------------------------

Expression* ArithmeticExpressionSemanticAction(Expression* left, Expression* right, ArithmeticOperator operator) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Expression* expression = malloc(sizeof(Expression));

    if (!expression) { return NULL; }

    expression->type = ARITHMETIC_EXPRESSION;
    expression->arithmetic = malloc(sizeof(*(expression->arithmetic)));

    if (!expression->arithmetic) {
        free(expression);
        return NULL;
    }
    expression->arithmetic->operator = operator;
    expression->arithmetic->left = left;
    expression->arithmetic->right = right;

    return expression;
}

Expression* FactorExpressionSemanticAction(Factor* factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Expression* expression = malloc(sizeof(Expression));

    if (!expression) { return NULL; }

    expression->type = FACTOR_EXPRESSION;
    expression->factor = factor;

    return expression;
}

Expression* RandomExpressionSemanticAction(Expression* min, Expression* max, Expression* charset) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Expression* expression = malloc(sizeof(Expression));

    if (!expression) { return NULL; }

	expression->type = EXPRESSION_RND;
    expression->random = malloc(sizeof(*(expression->random)));
    
    if (!expression->random) {
        free(expression);
        return NULL;
    }

	expression->random->min = min;
	expression->random->max = max;
	expression->random->charset = charset;

    return expression;
}

Expression* ReverseExpressionSemanticAction(Expression* input) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Expression* expression = malloc(sizeof(Expression));

    if (!expression) { return NULL; }

    expression->type = EXPRESSION_REV;
    expression->unary = malloc(sizeof(*(expression->unary)));

    if (!expression->unary) {
        free(expression);
        return NULL;
    }

	expression->type = EXPRESSION_REV;
	expression->unary->input = input;

    return expression;
}

Expression* ToUpperExpressionSemanticAction(Expression* input) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Expression* expression = malloc(sizeof(Expression));

    if (!expression) { return NULL; }

	expression->type = EXPRESSION_TUP;
    expression->unary = malloc(sizeof(*(expression->unary)));

    if (!expression->unary) {
        free(expression);
        return NULL;
    }
    expression->unary->input = input;

    return expression;
}

Expression* ToLowerExpressionSemanticAction(Expression* input) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Expression* expression = malloc(sizeof(Expression));

    if (!expression) { return NULL; }

    expression->type = EXPRESSION_TLO;
    expression->unary = malloc(sizeof(*(expression->unary)));

    if (!expression->unary) {
        free(expression);
        return NULL;
    }

	expression->unary->input = input;

    return expression;
}

Expression* ReplaceExpressionSemanticAction(Expression* original, Expression* target, Expression* replacement) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Expression* expression = malloc(sizeof(Expression));

    if (!expression) { return NULL; }

	expression->type = EXPRESSION_RPL;
    expression->replace = malloc(sizeof(*(expression->replace)));
    
    if (!expression->replace) {
        free(expression);
        return NULL;
    }

	expression->replace->original = original;
	expression->replace->target = target;
	expression->replace->replacement = replacement;

    return expression;
}

// ---------------------------------------------------------------------------------------

Factor* ConstantFactorSemanticAction(Constant* constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Factor* factor = malloc(sizeof(Factor));

    if (!factor) { return NULL; }

    factor->type = CONSTANT_FACTOR;
    factor->constant = constant;

    return factor;
}

Factor* ExpressionFactorSemanticAction(Expression* expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Factor* factor = malloc(sizeof(Factor));

    if (!factor) { return NULL; }

    factor->type = EXPRESSION_FACTOR;
    factor->expression = expression;

    return factor;
}

Factor* InterpolationFactorSemanticAction(Interpolation* interpolation) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Factor* factor = malloc(sizeof(Factor));

    if (!factor) { return NULL; }

    factor->type = INTERPOLATION_FACTOR;
    factor->interpolation = interpolation;

    return factor;
}

Factor* IdentifierFactorSemanticAction(char* identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	Factor* factor = malloc(sizeof(Factor));
	if (!factor) { return NULL; }

	factor->type = IDENTIFIER_FACTOR;
	factor->identifier = identifier;

	return factor;
}

// ---------------------------------------------------------------------------------------

Constant* AtomicConstantSemanticAction(int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Constant* constant = malloc(sizeof(Constant));

    if (!constant) { return NULL; }

    constant->type = ATOMIC_TYPE;
    constant->atomic = value;

    return constant;
}

Constant* StringConstantSemanticAction(char* value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Constant* constant = malloc(sizeof(Constant));

    if (!constant) { return NULL; }

    constant->type = STRING_TYPE;
    constant->string = value;

    return constant;
}

Constant* BufferConstantSemanticAction(Buffer* buffer) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Constant* constant = malloc(sizeof(Constant));

    if (!constant) { return NULL; }

    constant->type = BUFFER_TYPE;
    constant->buffer = buffer;

    return constant;
}

// ---------------------------------------------------------------------------------------

Declaration* StringDeclarationSemanticAction(char* identifier, char* literal) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Declaration* declaration = malloc(sizeof(Declaration));

    if (!declaration) { return NULL; }

    declaration->type = STRING_TYPE;
    declaration->identifier = identifier;
    declaration->stringLiteral = literal;

    return declaration;
}

Declaration* AtomicDeclarationSemanticAction(char* identifier, int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Declaration* declaration = malloc(sizeof(Declaration));

    if (!declaration) { return NULL; }

    declaration->type = ATOMIC_TYPE;
    declaration->identifier = identifier;
    declaration->atomicValue = value;

    return declaration;
}

Declaration* BufferDeclarationSemanticAction(char* identifier, Buffer* buffer) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

    Declaration* declaration = malloc(sizeof(Declaration));

    if (!declaration) { return NULL; }

    declaration->type = BUFFER_TYPE;
    declaration->identifier = identifier;
    declaration->bufferValue = buffer;

    return declaration;
}

Declaration* StringExpressionDeclarationSemanticAction(char* identifier, Expression* expression) { 
	_logSyntacticAnalyzerAction(__FUNCTION__);
	// Asignaciones a expresiones (para revisar)
    Declaration* declaration = malloc(sizeof(Declaration));

    if (!declaration) { return NULL; }

    declaration->type = STRING_TYPE;
    declaration->identifier = identifier;
    declaration->expression = expression;

    return declaration;
}

// ---------------------------------------------------------------------------------------

Interpolation* InterpolationSemanticAction(InterpolationFragmentList* list) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    Interpolation* interpolation = malloc(sizeof(Interpolation));

    if (!interpolation) { return NULL; }
    
    interpolation->fragments = list;

    return interpolation;
}

InterpolationFragmentList* InterpolationFragmentListSemanticAction(InterpolationFragmentList* next, InterpolationFragment* fragment) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    InterpolationFragmentList* list = malloc(sizeof(InterpolationFragmentList));
    if (!list) { return NULL; }

    list->head = fragment;
    list->next = next;

    return list;
}

// ---------------------------------------------------------------------------------------

InterpolationFragment* LiteralFragmentSemanticAction(char* literal) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	InterpolationFragment* fragment = malloc(sizeof(InterpolationFragment));

	if (!fragment) { return NULL; }

	fragment->type = LITERAL_FRAGMENT;
	fragment->literal = literal;
	fragment->next = NULL;

	return fragment;
}

InterpolationFragment* ExpressionFragmentSemanticAction(char* identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    logError(_logger, identifier);

	InterpolationFragment* fragment = malloc(sizeof(InterpolationFragment));

	if (!fragment) { return NULL; }

	fragment->type = EXPRESSION_FRAGMENT;
	fragment->identifier = identifier;
	fragment->next = NULL;

	return fragment;
}

// ---------------------------------------------------------------------------------------
