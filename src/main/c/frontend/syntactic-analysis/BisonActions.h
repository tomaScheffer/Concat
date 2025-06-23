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

Program* ProgramSemanticAction(StatementList* statements);
StatementList* StatementListSemanticAction(StatementList* list, Statement* statement);

Statement* RoutineStatementSemanticAction(Routine* routine);
Statement* RoutineCallSemanticAction(char* identifier);
Statement* StringOperationStatementSemanticAction(StringOperation* operation);
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
Factor* ConstantFactorSemanticAction(Constant* constant);
Factor* ExpressionFactorSemanticAction(Expression* expression);
Factor* InterpolationFactorSemanticAction(Interpolation* interpolation);

Constant* AtomicConstantSemanticAction(int value);
Constant* StringConstantSemanticAction(char* value);
Constant* BufferConstantSemanticAction(Buffer* value);

StringOperation* ReverseStringOperationSemanticAction(Interpolation* interpolation);
StringOperation* ToUpperStringOperationSemanticAction(Interpolation* interpolation);
StringOperation* ToLowerStringOperationSemanticAction(Interpolation* interpolation);
StringOperation* ReplaceStringOperationSemanticAction(Interpolation* original, Interpolation* target, Interpolation* replacement);

Interpolation* InterpolationSemanticAction(InterpolationFragmentList* list);
InterpolationFragmentList* InterpolationFragmentListSemanticAction(InterpolationFragmentList* list, InterpolationFragment* fragment);

InterpolationFragment* LiteralFragmentSemanticAction(char* text);
InterpolationFragment* ExpressionFragmentSemanticAction(char* identifier);

#endif
