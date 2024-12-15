#ifndef PARSE_H
#define PARSE_H

#include "Lexer.h"
#include "ObjCode.h"
#include "Identifier.h"
#include "ParseTreeNode.h"

class Token;

class Parse
{
public:
	Parse();
	Parse(std::string);
	~Parse();
public:
	void move();
	void program();
	void block();
	void decls();
	void stmts();
	void cond();
	void expr();
	void term();
	void factor();
	void arry();
	void GrammerAnalyzier();
	void PrintTemineCode();
	int getArrayValue(std::string, int);
	ObjCode& getCode();
    void printParseTree(ParseTreeNode* node, int depth = 0);
    void printTree();
private:
    Parse(const Parse&)=delete;
    Parse& operator=(const Parse&)=delete;
	Lexer lex;
	Token* look;
	ObjCode icode;
	Identifier ident;
	ParseTreeNode* root;
};

#endif
