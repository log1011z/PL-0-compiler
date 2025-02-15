#include "Identifier.h"

Id::Id(){}

Id::Id(KIND _kind, int _level, int _addr,int _size) :
	kind(_kind), level(_level), addr(_addr),size(_size)
{
}

Id& Id::operator=(const Id& _id)
{
	if (this == &_id) return *this;
	this->kind = _id.kind;
	this->level = _id.level;
	this->addr = _id.addr;
	this->size = _id.size;
	return *this;
}

Identifier::Identifier() :currentLevel(0)
{
	currM.resize(10, 0);
}
