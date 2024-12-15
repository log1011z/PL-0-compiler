#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <vector>
#include <map>
#include <string>

enum KIND {VAR = 1, CONST=2, PROC=3,ARRAY=4};

struct Id
{
	Id();
	Id(KIND, int, int,int = 0);
	Id& operator=(const Id&);
	KIND kind;
	int value;
	int level;
	int addr;
	int size;
};

class Identifier
{
public:
	Identifier();
public:
	std::map<std::string, Id> id;
	std::vector<int> currM;
	int currentLevel;
private:
	Identifier(const Identifier&)=delete;
	Identifier& operator=(const Identifier&)=delete;
};

#endif // IDENTIFIER_H
