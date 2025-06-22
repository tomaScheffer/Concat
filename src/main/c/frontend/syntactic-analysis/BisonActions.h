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
Statement* OutStatementSemanticAction(Expression* expression);
Statement* DeclarationStatementSemanticAction(Declaration* declaration);

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

StringOperation* ReverseStringOperationSemanticAction(char* string);
StringOperation* ToUpperStringOperationSemanticAction(char* string);
StringOperation* ToLowerStringOperationSemanticAction(char* string);
StringOperation* ReplaceStringOperationSemanticAction(char* original, char* target, char* replacement);

Interpolation* InterpolationSemanticAction(InterpolationFragmentList* fragments);
InterpolationFragment* InterpolationFragmentListSemanticAction(InterpolationFragment* head, InterpolationFragment* next);
InterpolationFragment* LiteralFragmentSemanticAction(char* text);
InterpolationFragment* ExpressionFragmentSemanticAction(char* identifier);

#endif
