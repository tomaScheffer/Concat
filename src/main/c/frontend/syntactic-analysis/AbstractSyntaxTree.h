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
typedef enum StringOperationType StringOperationType;

typedef struct Buffer Buffer;
typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Interpolation Interpolation;
typedef struct InterpolationFragment InterpolationFragment;
typedef struct InterpolationFragmentList InterpolationFragmentList;
typedef struct StringOperation StringOperation;
typedef struct Declaration Declaration;
typedef struct StatementList StatementList;
typedef struct Statement Statement;
typedef struct Routine Routine;
typedef struct Program Program;

/**
* Node types for the Abstract Syntax Tree (AST).
*/

enum ExpressionType {
	FACTOR_EXPRESSION,
	ARITHMETIC_EXPRESSION
};

enum ArithmeticOperator {
	ADDITION,
	DIVISION,
	MULTIPLICATION,
	SUBTRACTION
};

enum StringOperationType {
	STRING_OP_REVERSE,
	STRING_OP_TO_UPPER,
	STRING_OP_TO_LOWER,
	STRING_OP_REPLACE
};

enum ConstantType {
	ATOMIC_TYPE,
	STRING_TYPE,
	BUFFER_TYPE
};

enum FactorType {
	CONSTANT_FACTOR,
	EXPRESSION_FACTOR,
	INTERPOLATION_FACTOR
};

enum StatementType {
	STATEMENT_DECLARATION,
	STATEMENT_EXPRESSION,
	STATEMENT_ROUTINE,
	STATEMENT_ROUTINE_CALL,
	STATEMENT_STRING_OPERATION,
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

struct Expression {
	ExpressionType type;

	union {
		Factor* factor;
		
		struct {
			ArithmeticOperator operator;
			Expression* leftExpression;
			Expression* rightExpression;
		};
	};

};

struct Factor {
	FactorType type;

	union {
		Constant* constant;
		Expression* expression;
		Interpolation* interpolation;
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

struct StringOperation {
	StringOperationType type;
	Interpolation* arg1;
	Interpolation* arg2;
	Interpolation* arg3;
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
		StringOperation* stringOperation;
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
