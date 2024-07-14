#pragma once
#include "Resource.h"
#include"Location.h"
class People:public Resource,public Location
{
public:
	People(std::pair<int, int> l) :Location(l) {}
};

