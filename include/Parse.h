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
	void program(ParseTreeNode* parent);
    void block(ParseTreeNode* parent);
    void decls(ParseTreeNode* parent);
    void stmts(ParseTreeNode* parent);
    void cond(ParseTreeNode* parent);
    void expr(ParseTreeNode *parent);
    void term(ParseTreeNode *parent);
    void factor(ParseTreeNode *parent);
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
