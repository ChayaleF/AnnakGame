#include "GroundObject.h"

GroundObject::GroundObject(GroundObjectType t, bool isComplate, std::pair<int, int>location) :Location(location), type(t), isComplate(isComplate) {}

std::string  GroundObject::typeToString(GroundObjectType type)
{
	switch (type) {
	case GroundObjectType::City:
		return "City";
	case GroundObjectType::Village:
		return "Village";
	case GroundObjectType::Road:
		return "Road";
	default:
		return "Unknown";
	}
}