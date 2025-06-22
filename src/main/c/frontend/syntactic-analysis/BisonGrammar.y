%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	char * string;
	int integer;
	Token token;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;

    Declaration * declaration;
	DeclarationList * declarationList;
	StringOperation * stringOperation;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <token> STRING_TYPE_TOKEN ATOMIC_TYPE_TOKEN BUFFER_TYPE_TOKEN
%token <token> INP_TOKEN RND_TOKEN REV_TOKEN TUP_TOKEN TLO_TOKEN RPL_TOKEN NUM_TOKEN LEN_TOKEN ENC_TOKEN DEC_TOKEN
%token <token> END_TOKEN FUN_TOKEN OUT_TOKEN
%token <token> STRING_START_TOKEN STRING_END_TOKEN
%token <string> STRING_TEXT_TOKEN IDENTIFIER_TOKEN
%token <integer> INTEGER_TOKEN

%token <token> ASSIGN_TOKEN COLON_TOKEN SEMICOLON_TOKEN COMMA_TOKEN
%token <token> OPEN_PAREN_TOKEN CLOSE_PAREN_TOKEN
%token <token> OPEN_BRACE_TOKEN CLOSE_BRACE_TOKEN

%token <token> ADD_TOKEN SUB_TOKEN MUL_TOKEN DIV_TOKEN

%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

%type <declaration> declaration
%type <declarationList> declarationList
%type <stringOperation> stringOperation

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD_TOKEN SUB_TOKEN
%left MUL_TOKEN DIV_TOKEN

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program:
	  declarationList												{ $$ = ProgramSemanticAction($1); }
	;

declarationList:
	  declaration													{ $$ = DeclarationListSemanticAction(NULL, $1); }
	| declarationList declaration									{ $$ = DeclarationListSemanticAction($1, $2); }
	;

declaration:
	  "String" IDENTIFIER_TOKEN '=' STRING_LITERAL_TOKEN ';'		{ $$ = StringDeclarationSemanticAction($2, $4); }
	| "String" IDENTIFIER_TOKEN '=' stringOperation ';'				{ $$ = StringOperationDeclarationSemanticAction($2, $4); }
	| "String" IDENTIFIER_TOKEN '=' expression ';'					{ $$ = StringExpressionDeclarationSemanticAction($2, $4); }
	| "Atomic" IDENTIFIER_TOKEN '=' expression ';'					{ $$ = AtomicDeclarationSemanticAction($2, $4); }
	;

stringOperation:
	  "REV" '(' STRING_LITERAL_TOKEN ')'							{ $$ = ReverseStringOperationSemanticAction($3); }
	| "TUP" '(' STRING_LITERAL_TOKEN ')'							{ $$ = ToUpperStringOperationSemanticAction($3); }
	| "TLO" '(' STRING_LITERAL_TOKEN ')'							{ $$ = ToLowerStringOperationSemanticAction($3); }
	| "RPL" '(' STRING_LITERAL_TOKEN ',' STRING_LITERAL_TOKEN ',' STRING_LITERAL_TOKEN ')' { $$ = ReplaceStringOperationSemanticAction($3, $5, $7); }
	;

expression:
	  expression ADD_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, ADDITION); }
	| expression SUB_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, SUBTRACTION); }
	| expression MUL_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, MULTIPLICATION); }
	| expression DIV_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, DIVISION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor:
	  OPEN_PAREN_TOKEN expression CLOSE_PAREN_TOKEN					{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant:
	  INTEGER_TOKEN													{ $$ = IntegerConstantSemanticAction($1); }
	;

%%