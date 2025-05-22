#ifndef FLEX_ACTIONS_HEADER  
#define FLEX_ACTIONS_HEADER  

#include "../../shared/Environment.h"  
#include "../../shared/Logger.h"  
#include "../../shared/String.h"  
#include "../../shared/Type.h"  
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>

/** Initialize module's internal state. */  
void initializeFlexActionsModule();  

/** Shutdown module's internal state. */  
void shutdownFlexActionsModule();  

/**  
 * Flex lexeme processing actions.  
 */  

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);  
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);  
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);  

Token KeywordLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);  
Token SymbolLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);  
Token IntegerLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);  
Token StringLiteralLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);  
Token IdentifierLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);  
Token UnknownLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);  

#endif
