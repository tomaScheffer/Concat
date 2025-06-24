%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {

	/** Terminals. */
	int integer;
	int atomic;
	char* string;
	Buffer* buffer;
	Token token;

	/** Non-terminals. */

	Constant* constant;
	Expression* expression;
	Factor* factor;
	Program* program;

	Routine* routine;
    Declaration* declaration;
	Statement* statement;
	StatementList* statementList;
	Interpolation* interpolation;
	InterpolationFragment* interpolation_fragment;
	InterpolationFragmentList* interpolation_fragment_list;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*	Need syntax tree for nexts steps
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
%destructor { releaseRoutine($$); } <routine>
%destructor { releaseDeclaration($$); } <declaration>
%destructor { releaseStatement($$); } <statement>
%destructor { releaseStatementList($$); } <statementList>
%destructor { releaseInterpolation($$); } <interpolation>
%destructor { releaseInterpolationFragment($$); } <interpolation_fragment>
%destructor { releaseInterpolationFragmentList($$); } <interpolation_fragment_list>
*/

/** Terminals. */

%token <token> STRING_TYPE_TOKEN ATOMIC_TYPE_TOKEN BUFFER_TYPE_TOKEN
%token <token> RND_TOKEN REV_TOKEN TUP_TOKEN TLO_TOKEN RPL_TOKEN LEN_TOKEN ECP_TOKEN
%token <token> FUN_TOKEN OUT_TOKEN
%token <token> STRING_START_TOKEN STRING_END_TOKEN

%token <atomic> ATOMIC_TOKEN
%token <string> STRING_TOKEN IDENTIFIER_TOKEN INTERPOLATED_IDENTIFIER_TOKEN ROUTINE_CALL_TOKEN
%token <buffer> BUFFER_TOKEN

%token <token> ASSIGN_TOKEN COLON_TOKEN SEMICOLON_TOKEN COMMA_TOKEN
%token <token> OPEN_PAREN_TOKEN CLOSE_PAREN_TOKEN
%token <token> OPEN_BRACE_TOKEN CLOSE_BRACE_TOKEN
%token <token> ADD_TOKEN SUB_TOKEN MUL_TOKEN DIV_TOKEN

%token <token> INTERPOLATION_OPEN_TOKEN INTERPOLATION_CLOSE_TOKEN

%token <token> UNKNOWN

/** Non-terminals. */

%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

%type <routine> routine
%type <statement> statement
%type <declaration> declaration
%type <statementList> statement_list
%type <interpolation> interpolation
%type <interpolation_fragment> interpolation_fragment
%type <interpolation_fragment_list> interpolation_fragment_list

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
	statement_list													{ $$ = ProgramSemanticAction(currentCompilerState(), $1); }
	;

statement_list:
	statement SEMICOLON_TOKEN										{ $$ = StatementListSemanticAction(NULL, $1); }
	| statement_list statement SEMICOLON_TOKEN						{ $$ = StatementListSemanticAction($1, $2); }

statement:
	declaration														{ $$ = DeclarationStatementSemanticAction($1); }
	| expression													{ $$ = ExpressionStatementSemanticAction($1); }
	| routine														{ $$ = RoutineStatementSemanticAction($1); }
	| OUT_TOKEN expression											{ $$ = OutStatementSemanticAction($2); }
	| ROUTINE_CALL_TOKEN											{ $$ = RoutineCallStatementSemanticAction($1); }

expression:
	  expression ADD_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, ADDITION); }
	| expression SUB_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, SUBTRACTION); }
	| expression MUL_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, MULTIPLICATION); }
	| expression DIV_TOKEN expression								{ $$ = ArithmeticExpressionSemanticAction($1, $3, DIVISION); }
	| RND_TOKEN OPEN_PAREN_TOKEN expression COMMA_TOKEN expression COMMA_TOKEN expression CLOSE_PAREN_TOKEN { $$ = RandomExpressionSemanticAction($3, $5, $7); }
	| REV_TOKEN OPEN_PAREN_TOKEN expression CLOSE_PAREN_TOKEN		{ $$ = ReverseExpressionSemanticAction($3); }
	| TUP_TOKEN OPEN_PAREN_TOKEN expression CLOSE_PAREN_TOKEN		{ $$ = ToUpperExpressionSemanticAction($3); }
	| TLO_TOKEN OPEN_PAREN_TOKEN expression CLOSE_PAREN_TOKEN		{ $$ = ToLowerExpressionSemanticAction($3); }
	| LEN_TOKEN OPEN_PAREN_TOKEN expression CLOSE_PAREN_TOKEN		{ $$ = LengthExpressionSemanticAction($3); }
	| RPL_TOKEN OPEN_PAREN_TOKEN expression COMMA_TOKEN expression COMMA_TOKEN expression CLOSE_PAREN_TOKEN { $$ = ReplaceExpressionSemanticAction($3, $5, $7); }
	| ECP_TOKEN OPEN_PAREN_TOKEN expression COMMA_TOKEN expression CLOSE_PAREN_TOKEN						{ $$ = EncryptionExpressionSemanticAction($3, $5); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor:
	  OPEN_PAREN_TOKEN expression CLOSE_PAREN_TOKEN					{ $$ = ExpressionFactorSemanticAction($2); }
	| IDENTIFIER_TOKEN												{ $$ = IdentifierFactorSemanticAction($1); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	| interpolation													{ $$ = InterpolationFactorSemanticAction($1); }
	;

constant:
	  ATOMIC_TOKEN													{ $$ = AtomicConstantSemanticAction($1); }
	| STRING_TOKEN													{ $$ = StringConstantSemanticAction($1); }
	| BUFFER_TOKEN 													{ $$ = BufferConstantSemanticAction($1); }
	;

routine:
	FUN_TOKEN IDENTIFIER_TOKEN OPEN_BRACE_TOKEN statement_list CLOSE_BRACE_TOKEN		{ $$ = RoutineSemanticAction($2, $4); }
	;

declaration:
	ATOMIC_TYPE_TOKEN IDENTIFIER_TOKEN ASSIGN_TOKEN ATOMIC_TOKEN 		{ $$ = AtomicDeclarationSemanticAction($2, $4); }
	| BUFFER_TYPE_TOKEN IDENTIFIER_TOKEN ASSIGN_TOKEN BUFFER_TOKEN		{ $$ = BufferDeclarationSemanticAction($2, $4); }
	| STRING_TYPE_TOKEN IDENTIFIER_TOKEN ASSIGN_TOKEN expression		{ $$ = StringExpressionDeclarationSemanticAction($2, $4); }
	;

interpolation:
	STRING_START_TOKEN interpolation_fragment_list STRING_END_TOKEN		{ $$ = InterpolationSemanticAction($2); }
	;

interpolation_fragment_list:
	  interpolation_fragment											{ $$ = InterpolationFragmentListSemanticAction(NULL, $1); }
	| interpolation_fragment_list interpolation_fragment				{ $$ = InterpolationFragmentListSemanticAction($1, $2); }
	;

interpolation_fragment:
	  STRING_TOKEN																		{ $$ = LiteralFragmentSemanticAction($1); }
	| INTERPOLATED_IDENTIFIER_TOKEN														{ $$ = ExpressionFragmentSemanticAction($1); }
	| INTERPOLATION_OPEN_TOKEN IDENTIFIER_TOKEN INTERPOLATION_CLOSE_TOKEN				{ $$ = ExpressionFragmentSemanticAction($2); }
	;

%%