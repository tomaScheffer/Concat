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
typedef struct StringOperation StringOperation;
typedef struct Declaration Declaration;
typedef struct StatementList StatementList;
typedef struct Statement Statement;
typedef struct Routine Routine;
typedef struct Program Program;

/**
* Node types for the Abstract Syntax Tree (AST).
*/

// ----------------------------------------------------

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

enum ConstantType {
	INTEGER_TYPE,
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
	STATEMENT_ROUTINE,
	STATEMENT_ROUTINE_CALL,
	STATEMENT_OUTPUT
};

enum InterpolationFragmentType {
	LITERAL_FRAGMENT,
	EXPRESSION_FRAGMENT
};

// ----------------------------------------------------

struct Constant {
	ConstantType type;

	union {
		int integer;
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

enum DeclarationType {
	DECL_STRING_LITERAL,
	DECL_ATOMIC,
	DECL_BUFFER,
	DECL_EXPRESSION
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

struct Interpolation {
	InterpolationFragment* fragments;
};

struct StringOperation {
	char* operationName;
	char* arg1;
	char* arg2;
	char* arg3;
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
} StatementList;

struct Statement {
	StatementType type;

	union {
		Declaration* declaration;
		struct Routine* routine;
		char* routineCallName;
		Expression* outputExpression;
	};
};

struct Routine {
	char* identifier;
	StatementList* body;
} Routine;

struct Program {
	StatementList* statements;
};

// ----------------------------------------------------

/**
* Node recursive destructors.
*/
void releaseConstant(Constant* constant);
void releaseExpression(Expression* expression);
void releaseFactor(Factor* factor);
void releaseStringOperation(StringOperation* operation);
void releaseDeclaration(Declaration* declaration);
void releaseProgram(Program* program);

#endif
