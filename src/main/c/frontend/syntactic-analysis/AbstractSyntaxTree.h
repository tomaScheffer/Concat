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
typedef enum FactorType FactorType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct StringOperation StringOperation;
typedef struct Declaration Declaration;
typedef struct DeclarationList DeclarationList;
typedef struct Program Program;

typedef enum ValueType {
	INTEGER_TYPE,
	STRING_TYPE,
	BUFFER_TYPE
} ValueType;

/**
* Node types for the Abstract Syntax Tree (AST).
*/
enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
};

enum FactorType {
	CONSTANT,
	EXPRESSION,
	VARIABLE_REFERENCE
};

struct Buffer {
	int lenght;
	uint8_t *data;
};

struct Constant {
	ValueType type;

	union {
		int integer;
		char *string;
		struct Buffer* buffer;
	};
};

struct Factor {
	FactorType type;
	union {
		Constant* constant;
		Expression* expression;
	};
};

struct Expression {
	union {
		Factor* factor;
		struct {
			Expression* leftExpression;
			Expression* rightExpression;
		};
	};
	ExpressionType type;
};

struct Program {
	Expression * expression;
};

/**
* Node recursive destructors.
*/
void releaseConstant(Constant* constant);
void releaseExpression(Expression* expression);
void releaseFactor(Factor* factor);
void releaseStringOperation(StringOperation* operation);
void releaseDeclaration(Declaration* declaration);
void releaseDeclarationList(DeclarationList* list);
void releaseProgram(Program* program);

#endif
