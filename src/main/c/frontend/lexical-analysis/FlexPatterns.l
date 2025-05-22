%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%
"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"String"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), STRING_TOKEN); }
"Atomic"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ATOMIC_TOKEN); }
"Buffer"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), BUFFER_TOKEN); }

"INP"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), INP_TOKEN); }
"RND"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), RND_TOKEN); }
"REV"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), REV_TOKEN); }
"TUP"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), TUP_TOKEN); }
"TLO"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), TLO_TOKEN); }
"RPL"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), RPL_TOKEN); }
"NUM"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), NUM_TOKEN); }
"LEN"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), LEN_TOKEN); }
"ENC"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ENC_TOKEN); }
"DEC"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), DEC_TOKEN); }
"OUT"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), OUT_TOKEN); }

"FOR"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FOR_TOKEN); }
"END"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), END_TOKEN); }
"FUN"								{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FUN_TOKEN); }

\"([^\"\\]|\\.)*\"					{ return StringLiteralLexemeAction(createLexicalAnalyzerContext(), STRING_LITERAL_TOKEN); }
[0-9]+								{ return IntegerLexemeAction(createLexicalAnalyzerContext(), INTEGER_TOKEN); }

[A-Za-z_][A-Za-z0-9_]*				{ return IdentifierLexemeAction(createLexicalAnalyzerContext()); }

"="									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), ASSIGN_TOKEN); }
":"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), COLON_TOKEN); }
";"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), SEMICOLON_TOKEN); }
","									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), COMMA_TOKEN); }
"("									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), OPEN_PAREN_TOKEN); }
")"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), CLOSE_PAREN_TOKEN); }
"+"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), ADD_TOKEN); }
"-"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), SUB_TOKEN); }
"*"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), MUL_TOKEN); }
"/"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), DIV_TOKEN); }
"{"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE_TOKEN); }
"}"									{ return SymbolLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE_TOKEN); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.                                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
