#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token TypeLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

	return token;
}

Token KeywordLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

	return token;
}

Token SymbolLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

	return token;
}

Token AtomicLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->atomic = atoi(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

	return token;
}

Token StringLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	unsigned int len = lexicalAnalyzerContext->length;
	char* unquoted = strdup(lexicalAnalyzerContext->lexeme + 1);
	int lenght = strlen(lexicalAnalyzerContext->lexeme);
	unquoted[lenght - 2] = '\0';
	lexicalAnalyzerContext->semanticValue->string = unquoted;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

	return token;
}

Token IdentifierLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	
	lexicalAnalyzerContext->semanticValue->string = strdup(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

	return IDENTIFIER_TOKEN;
}

Token InterpolatedIdentifierLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    const char* lexeme = lexicalAnalyzerContext->lexeme;
    size_t len = strlen(lexeme);

    if (len >= 4 && lexeme[0] == '$' && lexeme[1] == '{' && lexeme[len - 1] == '}') {
        size_t content_len = len - 3;
        char* content = malloc(content_len + 1);
		
        if (!content) {
            destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
            return UNKNOWN;
        }
        strncpy(content, lexeme + 2, content_len);
        content[content_len] = '\0';

        lexicalAnalyzerContext->semanticValue->string = content;
    } else {
        lexicalAnalyzerContext->semanticValue->string = strdup(lexeme);
    }

	// logDebugging(_logger, lexicalAnalyzerContext->semanticValue->string);
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

    return INTERPOLATED_IDENTIFIER_TOKEN;
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

	return UNKNOWN;
}

Token BeginInterpolationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	
	return STRING_START_TOKEN;
}

Token EndInterpolationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}

	return STRING_END_TOKEN;
}