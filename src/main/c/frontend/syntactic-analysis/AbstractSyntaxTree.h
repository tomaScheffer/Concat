#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

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
	EXPRESSION
};

struct Constant {
	int value;
};

struct Factor {
	union {
		Constant* constant;
		Expression* expression;
	};
	FactorType type;
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

struct StringOperation {
	char* operationName;
	char* arg1;
	char* arg2;
	char* arg3;
};

struct Declaration {
	char* identifier;
	int isString; // 0 = expression, 1 = string literal, 2 = string operation (TODO: hacer enum)
	union {
		Expression* expression;
		char* stringLiteral;
		StringOperation* stringOperation;
	};
};

struct DeclarationList {
	Declaration* declaration;
	DeclarationList* next;
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
