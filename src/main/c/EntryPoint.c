#include "backend/code-generation/Generator.h"
#include "backend/domain-specific/Calculator.h"
#include "backend/semantic-analysis/SemanticAnalyzer.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/CompilerState.h"
#include "shared/Environment.h"
#include "shared/Logger.h"
#include "shared/String.h"


/**
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
const int main(const int count, const char ** arguments) {
	Logger * logger = createLogger("EntryPoint");

	initializeFlexActionsModule();
	initializeBisonActionsModule();
	initializeSyntacticAnalyzerModule();
	initializeAbstractSyntaxTreeModule();
	initializeCalculatorModule();
	initializeGeneratorModule();

	// Logs the arguments of the application.
	for (int k = 0; k < count; ++k) {
		logDebugging(logger, "Argument %d: \"%s\"", k, arguments[k]);
	}

	// Begin compilation process.
	CompilerState compilerState = {
		.abstractSyntaxtTree = NULL,
		.succeed = false,
		.value = 0
	};
	const SyntacticAnalysisStatus syntacticAnalysisStatus = parse(&compilerState);			// FRONT
	
	CompilationStatus compilationStatus = SUCCEED;
	Program * program = compilerState.abstractSyntaxtTree;

	if (syntacticAnalysisStatus == ACCEPT) {												// BACK
		
		logDebugging(logger, "Parsing succeeded. Starting semantic analysis...");

		if (performSemanticAnalysis(program)) {
			logDebugging(logger, "Semantic analysis succeeded. Starting code generation...");

			//generate(&compilerState);
		}
		else {
			logError(logger, "Semantic analysis failed.");
			compilationStatus = FAILED;
		}
	}
	else {
		logError(logger, "The syntactic-analysis phase rejects the input program.");
		compilationStatus = FAILED;
	}

	logDebugging(logger, "Releasing AST resources...");
	releaseProgram(program);
	logDebugging(logger, "Releasing modules resources...");
	shutdownGeneratorModule();
	shutdownCalculatorModule();
	shutdownAbstractSyntaxTreeModule();
	shutdownSyntacticAnalyzerModule();
	shutdownBisonActionsModule();
	shutdownFlexActionsModule();
	logDebugging(logger, "Compilation is done.");
	destroyLogger(logger);

	return syntacticAnalysisStatus;
}