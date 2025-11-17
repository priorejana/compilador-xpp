#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
		SymbolTable* globalST;
    	SymbolTable* currentST;

		void advance();
		void match(int);
		void error(string);
		void initSimbolTable();

		void program();
		void classList();
		void classDecl();
		void classBody();
		void classContentListOpt();
		void classContentList();
		void classContent();
		void varOrMethod();
		void varDeclList();
		void varDecl();
		void varDeclOpt();
		void type();
		void constructDecl();
		void methodBody();
		void paramListOpt();
		void paramList();
		void param();
		void statementsOpt();
		void statements();
		void statement();
		void atribStat();
		void printStat();
		void readStat();
		void returnStat();
		void superStat();
		void ifStat();
		void forStat();
		void atribStatOpt();
		void expressionOpt();
		void expression();
		void lValueComp();
		void lValue();
		void allocExpression();
		void numExpression();
		void unaryExpression();
		void term();
		void factor();
		void argListOpt();
		void argList();
	public:
		Parser(string);
		void run();
};