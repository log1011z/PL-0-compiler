#include <iostream>
#include <cctype>

#include "Lexer.h"
#include "Num.h"
#include "Word.h"
#include "Symbol.h"
#include "Error.h"

Lexer::Lexer() : line(1) {}

Lexer::Lexer(std::string FileName) : line(1)
{
	// 读取文件内容并将其转换为小写
        std::ifstream input(FileName);
        if (!input) {
            std::cerr << "error: unable to open input file -- "
                      << FileName
                      << std::endl;
            exit(0);
        }

        std::string tempFileName = FileName + ".tmp"; // 临时文件
        std::ofstream output(tempFileName);
        if (!output) {
            std::cerr << "error: unable to create temporary file -- "
                      << tempFileName
                      << std::endl;
            exit(0);
        }

        char ch;
        while (input.get(ch)) {
            output.put(std::tolower(static_cast<unsigned char>(ch))); // 转为小写
        }

        input.close();
        output.close();

        // 将临时文件重命名为原文件（覆盖原文件）
        if (std::rename(tempFileName.c_str(), FileName.c_str()) != 0) {
            std::cerr << "error: unable to overwrite the original file -- "
                      << FileName
                      << std::endl;
            exit(0);
        }

	const char* FileN = FileName.c_str();
	   in.open(FileN);
	if (!in){
		std::cerr << "error: unable to open input file -- "
				  << FileN 
				  << std::endl;
		exit(0);
	}
	
}

Lexer::~Lexer()
{
	for (auto token : TokenList)
		delete token;
	in.close();
}

void Lexer::PlusLine()
{
	++line;
}

void Lexer::Tokenizer()
{
	Token* p = 0;
	char nowChar = 0;
	auto putNumber = [&] {
		std::string s;
		do{
			s += nowChar;
			if (in.peek() == EOF) {
				p = new Num(s, line);
				return;
			}
			in.get(nowChar);
		} while (isdigit(nowChar));
		in.putback(nowChar);
		p = new Num(s, line);
	};
	auto putWord = [&] {
		std::string s;
		do{
			s += nowChar;
			if (in.peek() == EOF) {
				p = new Word(s, line);
				return;
			}
			in.get(nowChar);
		} while (isalnum(nowChar ));
		in.putback(nowChar);
		p = new Word(s, line);
	};
	auto putSymbol = [&] {
		std::string s(1, nowChar);
		if (s[0] == ':') {
			if (in.peek() == EOF) {
				p = new Symbol(s, line);
				return;
			}
			in.get(nowChar);
			if (nowChar == '=')		s = ":=";
			else	in.putback(nowChar);
		}
		if (s == "<" || s == ">") {
			if (in.peek() == EOF) {
				p = new Symbol(s, line);
				return;
			}
			in.get(nowChar);
			if (s == "<" && nowChar == '>')		s = "<>";
			else if (nowChar == '=')	s += "=";
			else	in.putback(nowChar);
		}
		p = new Symbol(s, line);
	};

	while (in.peek() != EOF)
	{
		in.get(nowChar);
		if (nowChar == '\n') {
			PlusLine();
			continue;
		}
		else if(nowChar == '{'){
			while (in.peek() != EOF && nowChar != '}')
				in.get(nowChar);
			if (in.peek() == EOF)
				printError(9, line);
			continue;
		}
		else if (isspace(nowChar)) {
			continue;
		}
		else if (isdigit(nowChar)) {
			putNumber();
		}
		else if (isalpha(nowChar)) {
			putWord();
		} 
		else {
			putSymbol();
		}
		TokenList.push_back(p);
	}
}

void Lexer::PrintList()
{
	for (auto token : TokenList)
		token->Print();
}

Token* Lexer::GetToken()
{
	if (TokenList.size()){
		Token* token = TokenList.front();
		TokenList.pop_front();
		return token;
	}
	return NULL;
}

void Lexer::PutToken(Token*& p)
{
	TokenList.push_front(p);
	p = 0;
}

void Lexer::JudgeError()
{
	if (!TokenList.size())
		printError(33, 1);
	std::list<Token*>::iterator it = TokenList.begin();
	for (; it != TokenList.end(); ++it){
		if ((*it)->GetTag() == BADTOKEN)
			printError(32, (*it)->GetLine());
	}
	--it;
	if ((*it)->GetTag() != PERIODSYM)
		printError(9, (*it)->GetLine());
}
