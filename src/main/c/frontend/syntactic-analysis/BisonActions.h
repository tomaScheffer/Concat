#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program* ProgramSemanticAction(CompilerState *compilerState, StatementList* statements);
StatementList* StatementListSemanticAction(StatementList* next, Statement* statement);

Statement* RoutineStatementSemanticAction(Routine* routine);
Statement* RoutineCallStatementSemanticAction(char* identifier);
Statement* OutStatementSemanticAction(Expression* expression);
Statement* DeclarationStatementSemanticAction(Declaration* declaration);
Statement* ExpressionStatementSemanticAction(Expression* expression);

Routine* RoutineSemanticAction(char* id, StatementList* body);
Declaration* StringDeclarationSemanticAction(char* id, char* value);
Declaration* AtomicDeclarationSemanticAction(char* id, int value);
Declaration* BufferDeclarationSemanticAction(char* id, Buffer* buffer);
Declaration* StringExpressionDeclarationSemanticAction(char* id, Expression* expr);

Expression* FactorExpressionSemanticAction(Factor* factor);
Expression* ArithmeticExpressionSemanticAction(Expression* left, Expression* right, ArithmeticOperator operator);
Expression* RandomExpressionSemanticAction(Expression* min, Expression* max, Expression* charset);
Expression* ReverseExpressionSemanticAction(Expression* input);
Expression* ToUpperExpressionSemanticAction(Expression* input);
Expression* ToLowerExpressionSemanticAction(Expression* input);
Expression* LengthExpressionSemanticAction(Expression* input);
Expression* ReplaceExpressionSemanticAction(Expression* original, Expression* target, Expression* replacement);
Expression* EncryptionExpressionSemanticAction(Expression* input, Expression* key);

Factor* ConstantFactorSemanticAction(Constant* constant);
Factor* ExpressionFactorSemanticAction(Expression* expression);
Factor* InterpolationFactorSemanticAction(Interpolation* interpolation);
Factor* IdentifierFactorSemanticAction(char* identifier);

Constant* AtomicConstantSemanticAction(int value);
Constant* StringConstantSemanticAction(char* value);
Constant* BufferConstantSemanticAction(Buffer* value);

Interpolation* InterpolationSemanticAction(InterpolationFragmentList* list);
InterpolationFragmentList* InterpolationFragmentListSemanticAction(InterpolationFragmentList* next, InterpolationFragment* fragment);

InterpolationFragment* LiteralFragmentSemanticAction(char* text);
InterpolationFragment* ExpressionFragmentSemanticAction(char* identifier);

#endif
