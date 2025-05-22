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
Program* ProgramSemanticAction(DeclarationList* declarations);
DeclarationList* AppendDeclarationListSemanticAction(DeclarationList* list, Declaration* declaration);
DeclarationList* EmptyDeclarationListSemanticAction();

Declaration* StringDeclarationSemanticAction(char* identifier, Expression* expression);
Declaration* AtomicDeclarationSemanticAction(char* identifier, Expression* expression);
Declaration* BufferDeclarationSemanticAction(char* identifier, Expression* expression);

Expression* StringOperationExpressionSemanticAction(StringOperation* operation);
Expression* IdentifierExpressionSemanticAction(char* identifier);
Expression* StringLiteralExpressionSemanticAction(char* literal);
Expression* IntegerLiteralExpressionSemanticAction(int value);
Expression* InputExpressionSemanticAction();

StringOperation* RndOperationSemanticAction(Expression* min, Expression* max, Expression* charset);
StringOperation* RevOperationSemanticAction(Expression* input);
StringOperation* TupOperationSemanticAction(Expression* input);
StringOperation* TloOperationSemanticAction(Expression* input);
StringOperation* RplOperationSemanticAction(Expression* original, Expression* from, Expression* to);
StringOperation* NumOperationSemanticAction(Expression* input);
StringOperation* LenOperationSemanticAction(Expression* input);
StringOperation* EncOperationSemanticAction(char* method, Expression* key, Expression* data);
StringOperation* DecOperationSemanticAction(char* method, Expression* key, Expression* encrypted);

#endif
