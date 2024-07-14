#pragma once
#include "GroundObject.h"

class Road : public GroundObject
{
private:
	static int roadCounter;

public:
	Road(GroundObjectType type, bool isComplate, std::pair<int, int>location);
	static int getRoadCounter() { return roadCounter; }
	void makeEmpty()override {}
};