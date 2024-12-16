#include <iostream>
#include <sstream>
#include <cstdlib>

#include "Parse.h"
#include "Error.h"
#include "Token.h"
#include "Identifier.h"

Parse::Parse() :look(0){}

Parse::Parse(std::string FileName) :lex(FileName), look(0),root(nullptr)
{
	lex.Tokenizer();
	//lex.PrintList();
}

Parse::~Parse(){
	delete root;
}


void Parse::move()
{
	if (look != 0) {
		delete look;
		look = NULL;
	}
	look = lex.GetToken();
}

void Parse::GrammerAnalyzier()
{
	lex.JudgeError();
	root = new ParseTreeNode("Program", "");
	program(root);
	printSymbolTable();
	//icode.printCode();
	//std::cout << "Over!" << std::endl;
}



void Parse::printParseTree(ParseTreeNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << node->type << ": " << node->value << std::endl;
    for (auto child : node->children) {
        printParseTree(child, depth + 1);
    }
}

void Parse::printSymbolTable() {
    std::cout << "Symbol Table:" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Name\tKind\tLevel\tAddress\tSize\tValue" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (const auto& entry : ident.id) {
        std::cout << entry.first << "\t";  

        switch (entry.second.kind) {
            case VAR:
                std::cout << "VAR\t";
                break;
            case CONST:
                std::cout << "CONST\t";
                break;
            case PROC:
                std::cout << "PROC\t";
                break;
            case ARRAY:
                std::cout << "ARRAY\t";
                break;
            default:
                std::cout << "UNKNOWN\t";
                break;
        }

        std::cout << entry.second.level << "\t";
        std::cout << entry.second.addr << "\t";
        if(entry.second.kind == ARRAY){
			std::cout << entry.second.size<<"\t";
		}
		else
			std::cout << "\t";
		if (entry.second.kind == CONST) {
            std::cout << entry.second.value<<"\t";
        }
		
        std::cout << std::endl;
    }
    std::cout << "-------------------------------------------" << std::endl;
}


void Parse::printTree() {
    printParseTree(root);
}

ObjCode& Parse::getCode()
{
	return this->icode;
}

void Parse::program(ParseTreeNode* parent)
{
	ParseTreeNode* blockNode = new ParseTreeNode("Block", "");
    parent->children.push_back(blockNode);
	block(blockNode);
	icode.emitCode(OPR, 0, OPR_RET);
	move();
	if (look->GetTag() != PERIODSYM){
		printError(9, look->GetLine());
	}
}

void Parse::block(ParseTreeNode* parent)
{
	ParseTreeNode* declsNode = new ParseTreeNode("Declarations", "");
    ParseTreeNode* stmtsNode = new ParseTreeNode("Statements", "");
    parent->children.push_back(declsNode);
    parent->children.push_back(stmtsNode);
	ident.currM[ident.currentLevel] += 3;
	icode.emitCode(INC, 0, 3);
	decls(declsNode);
	stmts(stmtsNode);
}

void Parse::decls(ParseTreeNode* parent)
{
	auto vardecl = [&](ParseTreeNode* parent){ 
		int numOfVarDecl = 0;
		ParseTreeNode* varDeclNode = new ParseTreeNode("VarDecl", ""); // 创建变量声明节点
        parent->children.push_back(varDeclNode);
		while (true){
			move();
			if (look->GetTag() != IDENTSYM)
				printError(4, look->GetLine());
			if (ident.id.find(look->GetLexeme()) != ident.id.end())
				printError(29, look->GetLine());
			std::string name=look->GetLexeme();
			ident.id[look->GetLexeme()] = Id(VAR,
											 ident.currentLevel, 
											 ident.currM[ident.currentLevel]++);
			numOfVarDecl++;
			std::string varName = look->GetLexeme();
            ParseTreeNode* varNode = new ParseTreeNode("Variable",varName); // 为每个变量创建节点
            varDeclNode->children.push_back(varNode);
			move();


			//arry
			if(look->GetTag() == LPARENTSYM){
				varNode->type = "Array";
				move();
				if(look->GetTag() != NUMBERSYM)
					printError(2, look->GetLine());
				 int lowerBound = atoi(look->GetLexeme().c_str());
				std::string lower = look->GetLexeme();
				 ParseTreeNode* lowerBoundNode = new ParseTreeNode("LowerBound", lower);
                varNode->children.push_back(lowerBoundNode);
				move();
				if(look->GetTag() != COLONSYM)
					printError(30, look->GetLine());
				move();
				if(look->GetTag() != NUMBERSYM)
					printError(2, look->GetLine());
				int upperBound = atoi(look->GetLexeme().c_str());
				std::string upper = look->GetLexeme();
                ParseTreeNode* upperBoundNode = new ParseTreeNode("UpperBound", upper);
                varNode->children.push_back(upperBoundNode);
				move();
				if(look->GetTag() != RPARENTSYM)
					printError(22, look->GetLine());
				if (lowerBound > upperBound) {
                printError(36, look->GetLine());  // 数组上下界非法
            }
			 ident.id[name] = Id(ARRAY, ident.currentLevel, ident.currM[ident.currentLevel]-1, upperBound - lowerBound + 1);
				move();
			}

			//arry end

			if (look->GetTag() == SEMICOLOMSYM)
				break;
			else if (look->GetTag() == COMMASYM)
				continue;
			else
				printError(30, look->GetLine());
		}
		icode.emitCode(INC, 0, numOfVarDecl);
	};

	auto condecl = [&](ParseTreeNode* parent){
		ParseTreeNode* constDeclNode = new ParseTreeNode("ConstDecl", ""); // 创建常量声明节点
        parent->children.push_back(constDeclNode);
		while (true){
			move();
			if (look->GetTag() != IDENTSYM)
				printError(4, look->GetLine());
			if (ident.id.find(look->GetLexeme()) != ident.id.end())
				printError(29, look->GetLine());
			std::string idName = look->GetLexeme();
            ParseTreeNode* constNode = new ParseTreeNode("Constant", idName); // 常量节点
            constDeclNode->children.push_back(constNode);
			move();
			if (look->GetTag() == BECOMESSYM)
				printError(1, look->GetLine());
			if (look->GetTag() != EQLSYM)
				printError(2, look->GetLine());

			move();
			if (look->GetTag() != NUMBERSYM)
				printError(2, look->GetLine());
			ident.id[idName].kind = CONST;
			ident.id[idName].value = atoi(look->GetLexeme().c_str());
			ParseTreeNode* constValue = new ParseTreeNode("Value", look->GetLexeme());
            constNode->children.push_back(constValue);
			move();
			if (look->GetTag() == COMMASYM)
				continue;
			else if (look->GetTag() == SEMICOLOMSYM)
				break;
			else
				printError(30, look->GetLine());
		}
	};

	auto procdecl = [&](ParseTreeNode* parent){ 
		while (true){
			ParseTreeNode* procNode = new ParseTreeNode("Procedure", "");
            parent->children.push_back(procNode);
			move();
			if (look->GetTag() != IDENTSYM)
				printError(4, look->GetLine());
			if (ident.id.find(look->GetLexeme()) != ident.id.end())
				printError(29, look->GetLine());
			ident.id[look->GetLexeme()] = Id(PROC, 
											 ident.currentLevel++,		
											 icode.size() + 1);
			int tmpAdrr1 = icode.size();
			icode.emitCode(JMP, 0, 0);
			std::string procName = look->GetLexeme();
            procNode->value = procName;
			move();
			if (look->GetTag() != SEMICOLOMSYM)
				printError(5, look->GetLine());
			ParseTreeNode* blockNode = new ParseTreeNode("Block", "");
            procNode->children.push_back(blockNode);
			block(blockNode);
			icode.emitCode(OPR, 0, OPR_RET);
			ident.currentLevel--;
			icode.changeAdrr(tmpAdrr1, icode.size());
			
			move();
			if (look->GetTag() != SEMICOLOMSYM)
				printError(5, look->GetLine());
			
			move();
			if (look->GetTag() != PROCSYM){
				lex.PutToken(look);
				break;
			} else {
				continue;
			}
		}
	};

	move();
	switch (look->GetTag()){
	case VARSYM:	vardecl(parent);				break;
	case CONSTSYM:	condecl(parent);				break;
	case PROCSYM:	procdecl(parent);				break;
	default:		lex.PutToken(look);		return;
	}
	decls(parent);
}

void Parse::stmts(ParseTreeNode* parent)
{
	move();
	std::string tmp;
	int tmpIfAdrr1, tmpIfAdrr2;
	int tmpWhileAdrr1, tmpWhileAdrr2;
	
	switch (look->GetTag())
	{
	case IDENTSYM://assign
	{
		ParseTreeNode* assignNode = new ParseTreeNode("Assign", "");
            parent->children.push_back(assignNode);
		if (ident.id.find(look->GetLexeme()) == ident.id.end())
			printError(11, look->GetLine());
		if (ident.id[look->GetLexeme()].kind != VAR&&ident.id[look->GetLexeme()].kind != ARRAY)
			printError(12, look->GetLine());
		if(ident.id[look->GetLexeme()].kind == VAR)
			tmp = look->GetLexeme();
		if(ident.id[look->GetLexeme()].kind == ARRAY)
		{
			tmp = look->GetLexeme();
			//shuzushibie
			move();
		if (look->GetTag() == LPARENTSYM)
		{
			expr(assignNode);
			
			move();
			if (look->GetTag() != RPARENTSYM)
				printError(22, look->GetLine());
		}
		else{
			printError(24, look->GetLine());
		}
		}
		assignNode->value = tmp;
		move();
		if (look->GetTag() != BECOMESSYM)
			printError(13, look->GetLine());
		expr(assignNode);
		icode.emitCode(STO,abs(ident.id[tmp].level - ident.currentLevel), ident.id[tmp].addr);
		break;
	}
	case CALLSYM://call
	{
		move();
		if (look->GetTag() != IDENTSYM)
			printError(14, look->GetLine());
		if (ident.id.find(look->GetLexeme()) == ident.id.end())
			printError(11, look->GetLine());
		if (ident.id[look->GetLexeme()].kind != PROC)
			printError(15, (*look).GetLine());
		tmp = look->GetLexeme();
            ParseTreeNode* callNode = new ParseTreeNode("Call", tmp);
            parent->children.push_back(callNode);
		icode.emitCode(CAL,
					   abs(ident.id[tmp].level - ident.currentLevel),
					   ident.id[tmp].addr);
		break;
	}
	case BEGINSYM://begin
	{
		ParseTreeNode *beginNode = new ParseTreeNode("Begin", "");
            parent->children.push_back(beginNode);
		while (true){
			stmts(beginNode);
			move();
			if (look->GetTag() != ENDSYM && look->GetTag() != SEMICOLOMSYM)
				printError(5, look->GetLine());
			if (look->GetTag() == ENDSYM)
				break;
			else
				continue;
		}
		break;
	}
	case IFSYM://if
	{
		ParseTreeNode* ifNode = new ParseTreeNode("If", "");
            parent->children.push_back(ifNode);
            cond(ifNode);

		move();
		if (look->GetTag() != THENSYM)
			printError(16, look->GetLine());
		tmpIfAdrr1 = icode.size();
		icode.emitCode(JPC, 0, 0);
		stmts(ifNode);
		tmpIfAdrr2 = icode.size();
		icode.emitCode(JMP, 0, 0);
		icode.changeAdrr(tmpIfAdrr1, icode.size());
		
		move();
		if (look->GetTag() != ELSESYM)
			lex.PutToken(look);
		else
		{
			ParseTreeNode* elseNode = new ParseTreeNode("Else", "");
                ifNode->children.push_back(elseNode);
                stmts(elseNode);
		}
		icode.changeAdrr(tmpIfAdrr2, icode.size());
		break;
	}
	case WHILESYM://while
	{
		ParseTreeNode* whileNode = new ParseTreeNode("While", "");
            parent->children.push_back(whileNode);
		tmpWhileAdrr1 = icode.size();
		cond(whileNode);
		tmpWhileAdrr2 = icode.size();
		
		move();
		icode.emitCode(JPC, 0, 0);
		if (look->GetTag() != DOSYM)
			printError(18, look->GetLine());
		stmts(whileNode);
		icode.emitCode(JMP, 0, tmpWhileAdrr1);
		icode.changeAdrr(tmpWhileAdrr2, icode.size());
		break;
	}
	case READSYM://read
	{
		move();
		if (look->GetTag() != IDENTSYM)
			printError(27, look->GetLine());
		if (ident.id.find(look->GetLexeme()) == ident.id.end())
			printError(11, look->GetLine());
		if (ident.id[look->GetLexeme()].kind == PROC)
			printError(35, look->GetLine());
		icode.emitCode(SIO_IN, 0, 2);
		tmp = look->GetLexeme();
		ParseTreeNode* readNode = new ParseTreeNode("Read", tmp);
            parent->children.push_back(readNode);
		icode.emitCode(STO,
					   abs(ident.id[tmp].level - ident.currentLevel),
					   ident.id[tmp].addr);
		break;
	}
	case WRITESYM://write
	{
		ParseTreeNode* writeNode = new ParseTreeNode("Write", "");
            parent->children.push_back(writeNode);
            move();
		if (look->GetTag() == LPARENTSYM)
		{
			expr(writeNode);
			while (true){
				move();
				if (look->GetTag() == COMMASYM){
				expr(writeNode);
			} else {
				lex.PutToken(look);
				break;
			}
		}
			move();
			if (look->GetTag() != RPARENTSYM)
				printError(22, look->GetLine());
		}
		else
			printError(24, look->GetLine());
		icode.emitCode(SIO_OUT, 0, 1);
		break;
	}
	default:
	{
		lex.PutToken(look);
		break;
	}
	}
}

void Parse::cond(ParseTreeNode* parent)
{
	ParseTreeNode* condNode = new ParseTreeNode("Condition", "");
    parent->children.push_back(condNode);
	move();
	if (look->GetTag() != ODDSYM){
		lex.PutToken(look);
		expr(condNode);

		move();
		auto tmp = look->GetTag();
		expr(condNode);
		switch (tmp) {
		case EQLSYM:
			icode.emitCode(OPR, 0, OPR_EQL);
			break;
		case NEQSYM:
			icode.emitCode(OPR, 0, OPR_NEQ);
			break;
		case LESSYM:
			icode.emitCode(OPR, 0, OPR_LSS);
			break;
		case LEQSYM:
			icode.emitCode(OPR, 0, OPR_LEQ);
			break;
		case GTRSYM:
			icode.emitCode(OPR, 0, OPR_GTR);
			break;
		case GEQSYM:
			icode.emitCode(OPR, 0, OPR_GEQ);
			break;
		default:
			printError(20, look->GetLine());
			break;
		}
	} else {
		expr(condNode);
	}
}

void Parse::expr(ParseTreeNode* parent)
{
	ParseTreeNode* exprNode = new ParseTreeNode("Expression","");
    parent->children.push_back(exprNode);
	int op;
	move();
	if (look->GetTag() == MINUSYM)
		icode.emitCode(OPR, 0, OPR_NEG);
	if (look->GetTag() != PLUSSYM && 
		look->GetTag() != MINUSYM)
			lex.PutToken(look);
	term(exprNode);
	while (true){
		move();
		if (look->GetTag() == PLUSSYM ||
			look->GetTag() == MINUSYM){
			op = look->GetTag();
		} else {
			lex.PutToken(look);
			break;
		}
		term(exprNode);
		if (op == PLUSSYM)
			icode.emitCode(OPR, 0, OPR_ADD);
		if (op == MINUSYM)
			icode.emitCode(OPR, 0, OPR_SUB);
	}
	
	
}

void Parse::term(ParseTreeNode* parent)
{
	ParseTreeNode* termNode = new ParseTreeNode("Term", "");
    parent->children.push_back(termNode);
	int op;
	factor(termNode);
	while (true){
		move();
		if (look->GetTag() == MULSYM ||
			look->GetTag() == SLASHSYM){
			op = look->GetTag();
		} else {
			lex.PutToken(look);
			break;
		}
		factor(termNode);
		if (op == MULSYM)
			icode.emitCode(OPR, 0, OPR_MUL);
		if (op == SLASHSYM)
			icode.emitCode(OPR, 0, OPR_DIV);
	}
}

void Parse::factor(ParseTreeNode* parent)
{
	ParseTreeNode *factorNode = new ParseTreeNode("Factor", "");
    parent->children.push_back(factorNode);
	move();
	if (look->GetTag() == LPARENTSYM){
		expr(factorNode);
		move();
		if (look->GetTag() != RPARENTSYM)
			printError(22, look->GetLine());
	} else {
		if (look->GetTag() != IDENTSYM && look->GetTag() != NUMBERSYM)
			printError(23, look->GetLine());
	}
	std::string tmp = look->GetLexeme();
	if (look->GetTag() == IDENTSYM && ident.id[look->GetLexeme()].kind == CONST)
		icode.emitCode(LIT, 0, ident.id[look->GetLexeme()].value);
	if (look->GetTag() == IDENTSYM && ident.id[look->GetLexeme()].kind == VAR)
		icode.emitCode(LOD,
					   abs(ident.id[look->GetLexeme()].level - ident.currentLevel),
					   ident.id[look->GetLexeme()].addr);
	if (look->GetTag() == IDENTSYM && ident.id[look->GetLexeme()].kind == ARRAY)
	{
		move();
		if (look->GetTag() == LPARENTSYM)
		{
			expr(factorNode);
			move();
			if (look->GetTag() != RPARENTSYM)
				printError(22, look->GetLine());
		}
		else
			printError(24, look->GetLine());
		// icode.emitCode(LOD,
		// 			   abs(ident.id[look->GetLexeme()].level - ident.currentLevel),//SHUZU HAIMEIGAI
		// 			   ident.id[look->GetLexeme()].addr);
	}
	factorNode->value = tmp;	
	if (look->GetTag() == NUMBERSYM)
		icode.emitCode(LIT,0,atoi(look->GetLexeme().c_str()));
}
