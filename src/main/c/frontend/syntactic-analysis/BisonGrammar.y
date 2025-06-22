%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {

	/** Terminals. */

	int integer;
	char* string;
	Token token;

	/** Non-terminals. */

	Constant* constant;
	Expression* expression;
	Factor* factor;
	Program* program;

	Routine* routine;
	Statement* statement;
    Declaration* declaration;
	Interpolation* interpolation;
	StringOperation* stringOperation;
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
%token <token> RND_TOKEN REV_TOKEN TUP_TOKEN TLO_TOKEN RPL_TOKEN NUM_TOKEN LEN_TOKEN ENC_TOKEN DEC_TOKEN
%token <token> FUN_TOKEN OUT_TOKEN
%token <token> STRING_START_TOKEN STRING_END_TOKEN

%token <string> STRING_TOKEN IDENTIFIER_TOKEN
%token <integer> ATOMIC_TOKEN
%token <buffer> BUFFER_TOKEN

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

%type <routine> routine
%type <statement> statement
%type <statement> routine_call
%type <declaration> declaration
%type <interpolation> interpolation
%type <statementList> statement_list
%type <stringOperation> string_operation

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */

%left ADD_TOKEN SUB_TOKEN
%left MUL_TOKEN DIV_TOKEN
%left CLOSE_PAREN_TOKEN

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program:
	statement_list													{ $$ = ProgramSemanticAction($1); }
	;

statement_list:
	statement SEMICOLON_TOKEN										{ $$ = StatementSemanticAction($1); }
	| statement_list statement SEMICOLON_TOKEN						{ $$ = StatementListSemanticAction($1, $2); }

statement:
	variable_declaration											{ $$ = VariableDeclarationStatementSemanticAction($1); }
	operation_declaration											{ $$ = OperationDeclarationStatementSemanticAction($1); }
	routine_declaration												{ $$ = RoutineDeclarationStatementSemanticAction($1); }
	routine_call													{ $$ = RoutineCallSemanticAction($1); }

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
	| STRING_TOKEN													{ $$ = StringConstantSemanticAction($1); }
	| BUFFER_TYPE_TOKEN												{ $$ = BufferConstantSemanticAction($1); }
	;

routine_declaration:
	FUN_TOKEN IDENTIFIER_TOKEN OPEN_BRACE_TOKEN statement_list CLOSE_BRACE_TOKEN		{ $$ = RoutineDeclarationSemanticAction($2, $4); }
	;

routine_call:
	IDENTIFIER_TOKEN													{ $$ = RoutineCallSemanticAction($1); }
	;

declaration:
	  STRING_TYPE_TOKEN IDENTIFIER_TOKEN ASSIGN_TOKEN STRING_TOKEN 		{ $$ = StringDeclarationSemanticAction($2, $4); }
	| ATOMIC_TYPE_TOKEN IDENTIFIER_TOKEN ASSIGN_TOKEN ATOMIC_TOKEN 		{ $$ = AtomicDeclarationSemanticAction($2, $4); }
	| BUFFER_TYPE_TOKEN IDENTIFIER_TOKEN ASSIGN_TOKEN BUFFER_TOKEN		{ $$ = BufferDeclarationSemanticAction($2, $4); }
	;

stringOperation:
	  REV_TOKEN OPEN_BRACE_TOKEN STRING_TOKEN CLOSE_BRACE_TOKEN			{ $$ = ReverseStringOperationSemanticAction($3); }
	| TUP_TOKEN OPEN_BRACE_TOKEN STRING_TOKEN CLOSE_BRACE_TOKEN			{ $$ = ToUpperStringOperationSemanticAction($3); }
	| TLO_TOKEN OPEN_BRACE_TOKEN STRING_TOKEN CLOSE_BRACE_TOKEN			{ $$ = ToLowerStringOperationSemanticAction($3); }
	| RPL_TOKEN OPEN_BRACE_TOKEN STRING_TOKEN COMMA_TOKEN STRING_TOKEN COMMA_TOKEN STRING_TOKEN CLOSE_BRACE_TOKEN { $$ = ReplaceStringOperationSemanticAction($3, $5, $7); }
	;

%%