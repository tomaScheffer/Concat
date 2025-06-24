#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>
#include <stdint.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ExpressionType ExpressionType;
typedef enum DeclarationType DeclarationType;
typedef enum ArithmeticOperator ArithmeticOperator;
typedef enum ConstantType ConstantType;
typedef enum FactorType FactorType;
typedef enum StatementType StatementType;
typedef enum InterpolationFragmentType InterpolationFragmentType;

typedef struct Buffer Buffer;
typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Interpolation Interpolation;
typedef struct InterpolationFragment InterpolationFragment;
typedef struct InterpolationFragmentList InterpolationFragmentList;
typedef struct Declaration Declaration;
typedef struct StatementList StatementList;
typedef struct Statement Statement;
typedef struct Routine Routine;
typedef struct Program Program;

typedef struct ArithmeticExpression ArithmeticExpression;
typedef struct UnaryExpression UnaryExpression;
typedef struct RandomExpression RandomExpression;
typedef struct ReplaceExpression ReplaceExpression;

/**
* Node types for the Abstract Syntax Tree (AST).
*/

enum ExpressionType {
	FACTOR_EXPRESSION,
	ARITHMETIC_EXPRESSION,
	EXPRESSION_RND,
	EXPRESSION_REV,
	EXPRESSION_TUP,
	EXPRESSION_TLO,
	EXPRESSION_LEN,
	EXPRESSION_RPL
};

enum ArithmeticOperator {
	ADDITION,
	DIVISION,
	MULTIPLICATION,
	SUBTRACTION
};

enum ConstantType {
	ATOMIC_TYPE,
	STRING_TYPE,
	BUFFER_TYPE
};

enum FactorType {
	CONSTANT_FACTOR,
	EXPRESSION_FACTOR,
	INTERPOLATION_FACTOR,
	IDENTIFIER_FACTOR
};

enum StatementType {
	STATEMENT_DECLARATION,
	STATEMENT_EXPRESSION,
	STATEMENT_ROUTINE,
	STATEMENT_ROUTINE_CALL,
	STATEMENT_OUTPUT
};

enum InterpolationFragmentType {
	LITERAL_FRAGMENT,
	EXPRESSION_FRAGMENT
};

enum DeclarationType {
	DECL_STRING_LITERAL,
	DECL_ATOMIC,
	DECL_BUFFER,
	DECL_EXPRESSION
};

// ----------------------------------------------------

struct Constant {
	ConstantType type;

	union {
		int atomic;
		char* string;
		struct Buffer* buffer;
	};
};

struct ArithmeticExpression {
	ArithmeticOperator operator;
	Expression* left;
	Expression* right;
};

struct UnaryExpression {
	Expression* input;
};

struct RandomExpression {
	Expression* min;
	Expression* max;
	Expression* charset;
};

struct ReplaceExpression {
	Expression* original;
	Expression* target;
	Expression* replacement;
};

struct Expression {
	ExpressionType type;

	union {
		Factor* factor;
		ArithmeticExpression* arithmetic;
		UnaryExpression* unary;
		RandomExpression* random;
		ReplaceExpression* replace;
	};
};

struct Factor {
	FactorType type;

	union {
		Constant* constant;
		Expression* expression;
		Interpolation* interpolation;
		char* identifier;
	};
};

struct InterpolationFragment {
	InterpolationFragmentType type;

	union {
		char* literal;
		char* identifier;
	};
	InterpolationFragment* next;
};

struct InterpolationFragmentList {
    InterpolationFragment* head;
    InterpolationFragmentList* next;
};

struct Interpolation {
	InterpolationFragmentList* fragments;
};

struct Declaration {
	ConstantType type;
	char* identifier;

	union {
		char* stringLiteral;
		int atomicValue;
		Buffer* bufferValue;
		Expression* expression;
	};
};

struct StatementList {
	Statement* statement;
	StatementList* next;
};

struct Statement {
	StatementType type;

	union {
		char* routineCallName;
		Declaration* declaration;
		struct Routine* routine;
		Expression* expression;
	};
};

struct Routine {
	char* identifier;
	StatementList* body;
};

struct Program {
	StatementList* statements;
};

// ----------------------------------------------------

void initializeAbstractSyntaxTreeModule();
void shutdownAbstractSyntaxTreeModule();

Interpolation* createInterpolation();

void releaseProgram(Program* program);
void releaseStatementList(StatementList* list);
void releaseStatement(Statement* statement);
void releaseRoutine(Routine* routine);
void releaseExpression(Expression* expression);
void releaseFactor(Factor* factor);
void releaseInterpolation(Interpolation* interpolation);
void releaseInterpolationFragmentList(InterpolationFragmentList* list);

#endif
