#pragma once
#include "GroundObject.h"
#include "People.h"
#include "Tile.h"
#include "GroundTransportation.h"
#include "AirTransport.h"
enum class ObjectType {
    GroundObject,
    People,
    GroundTransportation,
    AirTransport
};
class Coordinate
{
private:
    Tile* tile;
    GroundObject* groundObject;
    People* people;
    GroundTransportation* groundTransportation;
    AirTransport* airTransport;
public:
    Coordinate(Tile* t) : tile(t) {
        groundObject = nullptr;
        people = nullptr;
        groundTransportation = nullptr;
        airTransport = nullptr;
    }
    Tile* getTile() const { return tile; }
    GroundObject* getGroundObject() const { return groundObject; }
    void setGroundObject(GroundObject* c) { groundObject = c; }
    People* getPeople() const { return people; }
    void setPeople(People* p) { people = p; }
    GroundTransportation* getGroundTransportation() const { return groundTransportation; }
    void setGroundTransportation(GroundTransportation* gt) { groundTransportation = gt; }
    AirTransport* getAirTransport() const { return airTransport; }
    void setAirTransport(AirTransport* at) { airTransport = at; }

    void setObject(ObjectType type, Location* obj) {
        switch (type) {
        case ObjectType::GroundObject:
            groundObject = static_cast<GroundObject*>(obj);
            break;
        case ObjectType::People:
            people = static_cast<People*>(obj);
            break;
        case ObjectType::GroundTransportation:
            groundTransportation = static_cast<GroundTransportation*>(obj);
            break;
        case ObjectType::AirTransport:
            airTransport = static_cast<AirTransport*>(obj);
            break;
        default:
            // Handle invalid type
            break;
        }
    }
};