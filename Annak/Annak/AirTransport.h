#pragma once
#include "Tile.h"
#include "Location.h"
enum class AirTransportType {
	Helicopter
};

class AirTransport :public Resource,public Location
{
protected:
	AirTransportType type;
	static int helicopterCounter;
	//std::pair<int, int> location;


public:
	AirTransport(AirTransportType t,std::pair<int,int>location);
	AirTransportType  getType() const { return type; }
	void addResource(std::string resource, int amount) override;
	//std::pair<int, int> getLocation() const { return location; }
	static std::string typeToString(AirTransportType type);

	static int getHelicopterCounter();

};