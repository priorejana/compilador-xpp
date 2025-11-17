#include "parser.h"

Parser::Parser(string input)
{
	currentST = globalST = new SymbolTable();
	initSimbolTable();

	scanner = new Scanner(input, globalST);
	lToken = scanner->nextToken();
}

void Parser::advance()
{
	lToken = scanner->nextToken();
}

void Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void Parser::run()
{
	program();
	if (lToken->name != END_OF_FILE)
	{
		error("Erro: Código inesperado encontrado após o fim do programa.");
	}

	cout << "Compilação encerrada com sucesso!\n";
}

void Parser::program()
{
	if (lToken->name == CLASS)
		classList();
	else
		error("Erro: O programa deve começar com 'class'.");
}

void Parser::classList()
{
	do
	{
		classDecl();
	} while (lToken->name == CLASS);
}

void Parser::classDecl()
{
	match(CLASS);
	match(ID);

	if (lToken->name == EXTENDS)
	{
		advance();
		match(ID);
	}

	classBody();
}

void Parser::classBody()
{
	currentST = new SymbolTable(currentST);
	match(SEP_LCHAVE);    
	classContentListOpt();
    match(SEP_RCHAVE);
	currentST = currentST->getParent();
}

void Parser::classContentListOpt()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID || lToken->name == CONSTRUCTOR)
	{
		classContentList();
	}
}

void Parser::classContentList()
{
	classContent();
	while (lToken->name == INT || lToken->name == STRING || lToken->name == ID || lToken->name == CONSTRUCTOR)
	{
		classContent();
	}
	
}

void Parser::classContent()
{
	if (lToken->name == CONSTRUCTOR)
    {
        constructDecl(); 
    }
    else if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
		type();
		if (lToken->name == SEP_LCOLC)
		{
			match(SEP_LCOLC);
			match(SEP_RCOLC);
		}
		match(ID);
		varOrMethod();
	}
	else 
	{
		error("Esperado um construtor, variável ou método.");
	}
}

void Parser::varOrMethod()
{
	if (lToken->name == SEP_LPAREN)
	{
		methodBody();
	}
	else
	{
		varDeclOpt();
		match(SEP_PONTOVIRG);
	} 
}

void Parser::varDeclList()
{
	varDecl();
	while (lToken->name == INT || lToken->name == STRING)
	{
		varDecl();
	}
}

void Parser::varDecl()
{
	type();
	if (lToken->name == SEP_LCOLC)
	{
		match(SEP_LCOLC);
		match(SEP_RCOLC);
	}
	match(ID);
	varDeclOpt();
	match(SEP_PONTOVIRG);
}

void Parser::varDeclOpt()
{
	if (lToken->name == SEP_VIRG)
	{
		match(SEP_VIRG);
		match(ID);
		varDeclOpt();
	}
}

void Parser::type()
{
	if (lToken->name == INT)
	{
		match(INT);
	}
	else if (lToken->name == STRING)
	{
		match(STRING);
	}
	else if (lToken->name == ID)
	{
		match(ID);
	}
	else
	{
		error("Tipo (int, string, ou ID) esperado.");
	}
}

void Parser::constructDecl()
{
	match(CONSTRUCTOR);
	methodBody();
}

void Parser::methodBody()
{
	currentST = new SymbolTable(currentST);

	match(SEP_LPAREN);
	paramListOpt();
	match(SEP_RPAREN);
	match(SEP_LCHAVE);
	statementsOpt();
	match(SEP_RCHAVE);

	currentST = currentST->getParent();
}

void Parser::paramListOpt()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		paramList();
	}
}

void Parser::paramList()
{
	param();
	while (lToken->name == SEP_VIRG)
	{
		match(SEP_VIRG);
		param();
	}
}

void Parser::param()
{
	type();
	match(ID);
}

void Parser::statementsOpt()
{
	if (lToken->name == ID || lToken->name == PRINT || lToken->name == READ ||
		lToken->name == RETURN || lToken->name == SUPER || lToken->name == IF ||
		lToken->name == FOR || lToken->name == BREAK || lToken->name == SEP_PONTOVIRG ||
		lToken->name == INT || lToken->name == STRING)
	{
		statements();
	}
}

void Parser::statements()
{
	do
	{
		statement();
	} while (lToken->name == ID || lToken->name == PRINT || lToken->name == READ || lToken->name == RETURN || lToken->name == SUPER || lToken->name == IF || lToken->name == FOR || lToken->name == BREAK || lToken->name == SEP_PONTOVIRG || lToken->name == INT || lToken->name == STRING);
}

void Parser::statement()
{
	switch (lToken->name)
	{
	case ID:
    case INT:
    case STRING:
        if (lToken->name == INT || lToken->name == STRING)
        {
            varDecl(); 
        }
        else
        {
			atribStat();
            match(SEP_PONTOVIRG);
        }
        break;
	case PRINT:
		printStat();
		match(SEP_PONTOVIRG);
		break;
	case READ:
		readStat();
		match(SEP_PONTOVIRG);
		break;
	case RETURN:
		returnStat();
		match(SEP_PONTOVIRG);
		break;
	case SUPER:
		superStat();
		match(SEP_PONTOVIRG);
		break;
	case IF:
		ifStat();
		break;
	case FOR:
		forStat();
		break;
	case BREAK:
		match(BREAK);
		match(SEP_PONTOVIRG);
		break;
	case SEP_PONTOVIRG:
		match(SEP_PONTOVIRG);
		break;
	default:
		error("Comando (statement) inválido ou inesperado.");
		break;
	}
}

void Parser::atribStat()
{
	lValue();
	match(OP_EQ);
	if (lToken->name == NEW || lToken->name == INT || lToken->name == STRING)
	{
		allocExpression();
	}
	else
	{
		expression();
	}
}

void Parser::printStat()
{
	match(PRINT);
	expression();
}

void Parser::readStat()
{
	match(READ);
	lValue();
}

void Parser::returnStat()
{
	match(RETURN);
	expression();
}

void Parser::superStat()
{
	match(SUPER);
	match(SEP_LPAREN);
	argListOpt();
	match(SEP_RPAREN);
}

void Parser::lValue()
{
	match(ID);
	lValueComp();
}

void Parser::lValueComp()
{
	while (lToken->name == SEP_PONTO || lToken->name == SEP_LCOLC || lToken->name == SEP_LPAREN)
	{
		if (lToken->name == SEP_PONTO)
		{
			match(SEP_PONTO);
			match(ID);
			if (lToken->name == SEP_LCOLC)
			{
				match(SEP_LCOLC);
				expression();
				match(SEP_RCOLC);
			}
			else if (lToken->name == SEP_LPAREN)
			{
				match(SEP_LPAREN);
				argListOpt();
				match(SEP_RPAREN);
			}
		}
		else if (lToken->name == SEP_LCOLC)
		{
			match(SEP_LCOLC);
			expression();
			match(SEP_RCOLC);
		}
		else if (lToken->name == SEP_LPAREN)
		{
			match(SEP_LPAREN);
            argListOpt();
            match(SEP_RPAREN);
		}
	}
}

void Parser::expression()
{
	numExpression();
	if (lToken->name == OP_LT || lToken->name == OP_GT || lToken->name == OP_LEQ || lToken->name == OP_GEQ || lToken->name == OP_EQEQ || lToken->name == OP_DIF)
	{
		advance();
		numExpression();
	}
}

void Parser::allocExpression()
{
	if (lToken->name == NEW)
	{
		match(NEW);
		match(ID);
		match(SEP_LPAREN);
		argListOpt();
		match(SEP_RPAREN);
	}
	else
	{
		type();
		match(SEP_LCOLC);
		expression();
		match(SEP_RCOLC);
	}
}

void Parser::numExpression()
{
	term();
	while (lToken->name == OP_MAIS || lToken->name == OP_MENOS)
	{
		advance();
		term();
	}
}

void Parser::term()
{
	unaryExpression();
	while (lToken->name == OP_MUL || lToken->name == OP_DIV || lToken->name == OP_MOD)
	{
		advance();
		unaryExpression();
	}
}

void Parser::unaryExpression()
{
	if (lToken->name == OP_MAIS || lToken->name == OP_MENOS)
	{
		advance();
	}
	factor();
}

void Parser::factor()
{
	switch (lToken->name)
	{
	case INTEGER_LITERAL:
		match(INTEGER_LITERAL);
		break;
	case STRING_LITERAL:
		match(STRING_LITERAL);
		break;
	case ID:
		lValue();
		break;
	case SEP_LPAREN:
		match(SEP_LPAREN);
		expression();
		match(SEP_RPAREN);
		break;
	default:
		error("Fator (número, string, ID ou expressão) esperado.");
	}
}

void Parser::argListOpt()
{
	if (lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL || lToken->name == ID || lToken->name == SEP_LPAREN || lToken->name == OP_MAIS || lToken->name == OP_MENOS)
	{
		argList();
	}
}

void Parser::argList()
{
	expression();
	while (lToken->name == SEP_VIRG)
	{
		match(SEP_VIRG);
		expression();
	}
}

void Parser::ifStat()
{
	match(IF);
	match(SEP_LPAREN);
	expression();
	match(SEP_RPAREN);
	match(SEP_LCHAVE);
	statements();
	match(SEP_RCHAVE);
	if (lToken->name == ELSE)
	{
		match(ELSE);
		match(SEP_LCHAVE);
		statements();
		match(SEP_RCHAVE);
	}
}

void Parser::forStat()
{
	match(FOR);
	match(SEP_LPAREN);
	atribStatOpt();
	match(SEP_PONTOVIRG);
	expressionOpt();
	match(SEP_PONTOVIRG);
	atribStatOpt();
	match(SEP_RPAREN);
	match(SEP_LCHAVE);
	statements();
	match(SEP_RCHAVE);
}

void Parser::atribStatOpt()
{
	if (lToken->name == ID)
	{
		atribStat();
	}
}

void Parser::expressionOpt()
{
	if (lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL || lToken->name == ID || lToken->name == SEP_LPAREN || lToken->name == OP_MAIS || lToken->name == OP_MENOS)
	{
		expression();
	}
}

void Parser::initSimbolTable()
{
	Token *t;

	t = new Token(CLASS, "class");
	globalST->add(new STEntry(t, true));
	t = new Token(EXTENDS, "extends");
	globalST->add(new STEntry(t, true));
	t = new Token(INT, "int");
	globalST->add(new STEntry(t, true));
	t = new Token(STRING, "string");
	globalST->add(new STEntry(t, true));
	t = new Token(BREAK, "break");
	globalST->add(new STEntry(t, true));
	t = new Token(PRINT, "print");
	globalST->add(new STEntry(t, true));
	t = new Token(READ, "read");
	globalST->add(new STEntry(t, true));
	t = new Token(RETURN, "return");
	globalST->add(new STEntry(t, true));
	t = new Token(SUPER, "super");
	globalST->add(new STEntry(t, true));
	t = new Token(IF, "if");
	globalST->add(new STEntry(t, true));
	t = new Token(ELSE, "else");
	globalST->add(new STEntry(t, true));
	t = new Token(FOR, "for");
	globalST->add(new STEntry(t, true));
	t = new Token(NEW, "new");
	globalST->add(new STEntry(t, true));
	t = new Token(CONSTRUCTOR, "constructor");
	globalST->add(new STEntry(t, true));
}

void Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
