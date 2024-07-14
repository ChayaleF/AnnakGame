#pragma once
#include "Tile.h"
#include "Location.h"
enum class GroundObjectType {
	City,
	Village,
	Road
};

class GroundObject:public Location
{
protected:
	GroundObjectType type;
	bool isComplate;

public:
	GroundObject(GroundObjectType t, bool isComplate, std::pair<int, int>location);
	GroundObjectType  getType() const { return type; }
	virtual void makeEmpty() = 0;
	static std::string typeToString(GroundObjectType type);
	bool isComplete() { return isComplate; }

};