#include "GroundTransportation.h"

int GroundTransportation::carCounter=0;
int GroundTransportation::truckCounter=0;

GroundTransportation::GroundTransportation( GroundTransportationType t ,std::pair<int, int>location) : type(t), Location(location){

	if (type == GroundTransportationType::Car)
		carCounter++;
	if (type == GroundTransportationType::Truck)
		truckCounter++;
}

void GroundTransportation::addResource(std::string resource, int amount)
{
	if (resources.find(resource) != resources.end()) {
		resources[resource] = std::min(resources[resource] + amount, ReadJson::capacities[typeToString(type)][ReadJson::resourceTypes[resource]]);
	}
	else {
		resources[resource] = std::min(amount, ReadJson::capacities[typeToString(type)][ReadJson::resourceTypes[resource]]);
	}
}

std::string  GroundTransportation::typeToString(GroundTransportationType type)
{
	switch (type) {
	case GroundTransportationType::Car:
		return "Car";
	case GroundTransportationType::Truck:
		return "Truck";
	default:
		return "Unknown";
	}
}
int GroundTransportation::getCarCounter()
{
	return carCounter;
}

int GroundTransportation::getTruckCounter()
{
	return truckCounter;
}