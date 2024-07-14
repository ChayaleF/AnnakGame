#include "World.h"
#include "Road.h"
#include "Settlement.h"
#include <climits>

//init
void World::fillTileGrid(const std::vector<std::vector<std::string>>& data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        tileGrid.push_back(std::vector<Tile*>());
        for (size_t j = 0; j < data[i].size(); j++)
            tileGrid[i].push_back(new Tile(std::stoi(data[i][j])));
    }
}
void World::fillCellGrid()
{
    for (int i = 0; i < tileGrid.size() * ReadJson::sizes["Tile"][1]; i++)
    {
        cellGrid.push_back(std::vector<Coordinate*>());
        for (int j = 0; j < tileGrid[0].size() * ReadJson::sizes["Tile"][0]; j++)
        {
            cellGrid[i].push_back(new Coordinate(tileGrid[i / ReadJson::sizes["Tile"][1]][j / ReadJson::sizes["Tile"][0]]));
        }
    }
}
//bool World::insertResource(int amount, string resource, int x, int y)
//{
//    Tile* t = selectTile(x, y);
//    if (t->getTile().second < 3) {
//        return false;
//    }
//    if (((TileResource*)t)->getResourceType() != resource) {
//        return false;
//    }
//    ((TileResource*)t)->addResource(x - 1, y - 1, amount);
//    return true;
//}
//std::vector<int> World::selectedResource()
//{
//    std::vector<int> amount;
//    for (auto a : ReadJson::resourceTypes)
//    {
//        if (Resource::resourceCounter.find(a) != Resource::resourceCounter.end())
//            amount.push_back(Resource::resourceCounter[a]);
//        else
//            amount.push_back(0);
//    }
//    amount.pop_back();
//    return amount;
//}

//help
Coordinate* World::getCell(std::pair<int, int>cell)
{
    return cellGrid[cell.second - 1][cell.first - 1];
}
Coordinate* World::getCell(int x, int y)
{
    return cellGrid[y - 1][x - 1];
}
Tile*& World::selectTile(int x, int y)
{
    return tileGrid[(y - 1) / ReadJson::sizes["Tile"][0]][(x - 1) / ReadJson::sizes["Tile"][1]];
}
Settlement* World::isSettlement(std::pair<int, int> cell)
{
    if (getCell(cell)->getGroundObject()) {
        Settlement* s = dynamic_cast<Settlement*>(cellGrid[cell.second - 1][cell.first - 1]->getGroundObject());
        if (s) {
            return s;
        }
    }
    return nullptr;
}
Road* World::isRoad(std::pair<int, int> cell)
{
    if (getCell(cell)->getGroundObject()) {
        Road* r = dynamic_cast<Road*>(cellGrid[cell.second - 1][cell.first - 1]->getGroundObject());
        if (r) {
            return r;
        }
    }
    return nullptr;
}
bool World::canBuild(int x, int y, int sizeX, int sizeY) {
    int road = ReadJson::sizes["Road"][0];
    //up
    for (size_t i = x + road - 1; y-1 >= 0 && i < x + sizeX - road + 1&&i<cellGrid.size(); i++)
        if (cellGrid[y-1][i]->getGroundObject() && cellGrid[y-1 ][i]->getGroundObject()->getType() == GroundObjectType::Road)
            return true;
    //down
    for (size_t i = x + road - 1; y+1 + sizeY <cellGrid.size() && i < x + sizeX - road + 1 && i < cellGrid.size(); i++)
        if (cellGrid[y+1 + sizeY][i]->getGroundObject() && cellGrid[y + sizeY][i]->getGroundObject()->getType() == GroundObjectType::Road)
            return true;
    for (size_t i = y + road - 1; x-1 >= 0 && i < y + sizeY - road + 1 && i < cellGrid.size(); i++)
        if (cellGrid[i][x-1]->getGroundObject() && cellGrid[i][x-1]->getGroundObject()->getType() == GroundObjectType::Road)
            return true;
    for (size_t i = y + road - 1; x+1 + sizeX <cellGrid.size() && i < y + sizeY - road + 1 && i < cellGrid.size(); i++)
        if (cellGrid[i][x+1]->getGroundObject() && cellGrid[i][x+1 + sizeX]->getGroundObject()->getType() == GroundObjectType::Road)
            return true;
    return false;
}
bool World::isExistResource(std::vector<int> cost, std::unordered_map<string, int>exist) {

    for (auto res : ReadJson::resourceTypes)
        if (cost[res.second] > exist[res.first])
            return false;
    return true;
}
//start
void World::insertResource(int amount, string resource, int x, int y)
{
    Coordinate* currentCell = getCell(x, y);
    if (currentCell->getPeople()) {
        currentCell->getPeople()->addResource(resource, amount);
        return;
    }
    if (currentCell->getGroundTransportation()) {
        currentCell->getGroundTransportation()->addResource(resource, amount);
        return;
    }
    if (currentCell->getAirTransport()) {
        currentCell->getAirTransport()->addResource(resource, amount);
        return;
    }
    Settlement* s = isSettlement(std::pair<int, int>(x, y));
    if (s) {
        s->addResource(resource, amount);
        return;
    }
    currentCell->getTile()->addResource(resource, amount);
}
void World::insertResources(std::vector<int> resources, int x, int y) {
    //if this city or village
    //Coordinate* cell = getCell(x, y);
    Coordinate* cell = getCell(x, y);
    Resource* r=cell->getTile();
    if (cell->getPeople())
        r = cell->getPeople();
    else if (cell->getAirTransport())
        r = cell->getAirTransport();
    else if (cell->getGroundTransportation())
        r = cell->getGroundTransportation();
    else if (cell->getGroundObject()) {
        Settlement* s = isSettlement(std::pair<int, int>(x, y));
        if (s)
            r = s;
    }
    
    for (auto res : ReadJson::resourceTypes) {
        if(res.first!="People")
            r->addResource(res.first, resources[res.second]);
    }   
}
bool World::insertPeople(int amount, int x, int y) {

    Settlement* s = isSettlement(std::pair<int, int>(x, y));
    if (s)
        s->addPeople(amount);
    else
        getCell(x, y)->getTile()->addPeople(amount);
    People* p= new People(std::pair<int, int>(x - 1, y - 1));
    getCell(x, y)->setPeople(p);
    return putInCellGrid(p, ObjectType::People, ReadJson::sizes["People"]);
}
int World::buildGroundObject(std::string type, int x, int y, bool isComplate)
{
    /*if (cell[Gridy - 1][x - 1]->getGroundObject() != nullptr)
    return;*/
    GroundObject* g;
    if (type == GroundObject::typeToString(GroundObjectType::Road))
    {
        g = new Road(GroundObjectType::Road, isComplate, std::pair<int, int>(x - 1, y - 1));
        putInCellGrid(g, ObjectType::GroundObject, ReadJson::sizes[type]);
        return 0;
    }
    else if (type == GroundObject::typeToString(GroundObjectType::City)) {
        if (isComplate || canBuild(x - 1, y - 1, ReadJson::sizes[type][0], ReadJson::sizes[type][1]))
        {
            g=new Settlement(GroundObjectType::City, isComplate,std::pair<int,int>(x-1,y-1));
            std::vector<int> size = ReadJson::sizes[type];
            putInCellGrid(g, ObjectType::GroundObject,size);
            return 2;
        }
    }
    else if (type == GroundObject::typeToString(GroundObjectType::Village)) {
        if (isComplate || canBuild(x - 1, y - 1, ReadJson::sizes[type][0], ReadJson::sizes[type][1]))
        {
            g=new Settlement(GroundObjectType::Village, isComplate, std::pair<int, int>(x - 1, y - 1));
            putInCellGrid(g, ObjectType::GroundObject, ReadJson::sizes[type]);
            return 1;
        }
    }
    return 0;
    
}
bool World::manufacture(std::string type, int x, int y, int checkResource)
{
    std::unordered_map<string,int> exist;
    std::vector<int> cost = ReadJson::costs[type];

    Coordinate* currentCell = getCell(x, y);
    Settlement* s = isSettlement(std::pair<int, int>(x, y));
    if (s)
        exist = s->getResources();
    else
        exist=currentCell->getTile()->getResources();

    if (!(checkResource || isExistResource(cost, exist)))
        return false;

    if (type == AirTransport::typeToString(AirTransportType::Helicopter))
    {
        AirTransport* a = new AirTransport(AirTransportType::Helicopter, std::pair<int, int>(x - 1, y - 1));
        if (!putInCellGrid(a, ObjectType::AirTransport, ReadJson::sizes[type]))
            return false;
        if (s)
            s->addHelicopter(1);
        else
            currentCell->getTile()->addHelicopter(1);
        return true;
    }
    /* if (currentCell->getGroundTransportation() || currentCell->getPeople())
         return false;*/
    else if (type == GroundTransportation::typeToString(GroundTransportationType::Car))
    {
        GroundTransportation* g = new GroundTransportation(GroundTransportationType::Car, std::pair<int, int>(x - 1, y - 1));
        if (!putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes[type]))
            return false;
        if (s)
            s->addCar(1);
        else
            currentCell->getTile()->addCar(1);
        return true;
    }
    else if (type == GroundTransportation::typeToString(GroundTransportationType::Truck))
    {
        GroundTransportation* g = new GroundTransportation(GroundTransportationType::Truck, std::pair<int, int>(x - 1, y - 1));
        if (!putInCellGrid(g,ObjectType::GroundTransportation, ReadJson::sizes[type]))
            return false;
        if (s)
            s->addTruck(1);
        else
            currentCell->getTile()->addTruck(1);
        return true;
    }
    return false;
}
void World::makeEmpty(int x, int y) {
   // getCell(x - 1, y - 1)->getGroundObject();
    /* if (cellGrid[y - 1][x - 1]->getGroundObject())
         cellGrid[y - 1][x - 1]->getGroundObject()->makeEmpty();*/
}
//input
void World::peopleWork(std::pair<int, int> prev, std::pair<int, int> next)
{
    Coordinate* prevCell = getCell(prev);
    Coordinate* nextCell = getCell(next);
    People* p = prevCell->getPeople();
    putInCellGrid(ObjectType::People,ReadJson::sizes["People"], p->getLocation());
    p->setLocation(next);
    putInCellGrid(p,ObjectType::People, ReadJson::sizes["People"]);
    Settlement* s = isSettlement(prev);
    if (s)
        s->subPeople(1);
    else
        prevCell->getTile()->subPeople(1);
    nextCell->getTile()->addPeople(1);
    std::string resourceType = ReadJson::tilesResourceType[nextCell->getTile()->getTile().first];
    nextCell->getTile()->subResource(resourceType, 1);
    p->addResource(resourceType, 1);
}
void World::deposit(std::pair<int, int> prev, std::pair<int, int> next) {
    //פונקצייה זו נבנתה בהנחה ש: פריקה יכולה להתבצע רק בעיר  והכלי רכב/האיש יכול להגיע רק מטייל
    Coordinate* prevCell = getCell(prev);
    Coordinate* nextCell = getCell(next);
    Settlement* s = isSettlement(next);
    if (!s)
        return;
    Resource* src;
    std::vector <int> amount(ReadJson::tilesResourceType.size(), INT_MAX);

    if (prevCell->getPeople()) {
        src = prevCell->getPeople();
        prevCell->getTile()->subPeople(1);
        s->addPeople(1);
        People* p = prevCell->getPeople();
        p->setLocation(next);
        putInCellGrid(ObjectType::People, ReadJson::sizes["People"], prev);
        putInCellGrid(p, ObjectType::People, ReadJson::sizes["People"]);
    }
    else if (prevCell->getGroundTransportation()) {
        src = prevCell->getGroundTransportation();
        if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Car) {
            prevCell->getTile()->subCar(1);
            s->addCar(1);
            GroundTransportation* g = prevCell->getGroundTransportation();
            g->setLocation(next);
            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Car"], prev);
            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Car"]);
        }

        else if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Truck) {
            prevCell->getTile()->subTruck(1);
            s->addTruck(1);
            GroundTransportation* g = prevCell->getGroundTransportation();
            g->setLocation(next);
            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Truck"], prev);
            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Truck"]);
        }
        
        
    }
    else if (prevCell->getAirTransport()) {
        src = prevCell->getAirTransport();
        if (prevCell->getAirTransport()->getType() == AirTransportType::Helicopter) {
            prevCell->getTile()->subHelicopter(1);
            prevCell->getTile()->subCar(1);

        }
        AirTransport* a = prevCell->getAirTransport();
        a->setLocation(next);
        putInCellGrid(ObjectType::AirTransport, ReadJson::sizes["Helicopter"], prev);
        putInCellGrid(a, ObjectType::AirTransport, ReadJson::sizes["Helicopter"]);
    }
    else return;
    
    if (s->getType() == GroundObjectType::City) {
        amount= ReadJson::capacities[GroundObject::typeToString(GroundObjectType::City)];
    }
    else {
        amount = ReadJson::capacities[GroundObject::typeToString(GroundObjectType::Village)];

    }
    for (auto& rs : src->getResources()) {
        s->addResource(rs.first, min(amount[ReadJson::resourceTypes[rs.first]] - s->getResources()[rs.first], rs.second));
        src->addResource(rs.first, min(amount[ReadJson::resourceTypes[rs.first]] - s->getResources()[rs.first], rs.second));
    }
}
void World::takeResources(std::pair<int, int> prev, std::pair<int, int> next) {
    //פונקצייה זו נכתבה בהנחה ששכלי רכב/איש יכולים להגיע רק מטייל וטוענים מסע מעיר
    //מאיפה הכלי תחבורה/האיש הגיע
    Coordinate* prevCell = getCell(prev);
    //לאיפה הכלי תחבורה/האיש עובר
    Coordinate* nextCell = getCell(next);
    Settlement* s = isSettlement(next);
    if (!s)
        return;
    std::unordered_map<std::string, int> resourses = s->getResources();
    Resource* target;
    std::vector <int> amount(ReadJson::tilesResourceType.size(), INT_MAX);
    if (prevCell->getPeople()) {
        prevCell->getTile()->subPeople(1);
        nextCell->getTile()->addPeople(1);
        target = prevCell->getPeople();
        People* p = prevCell->getPeople();
        p->setLocation(next);
        putInCellGrid(ObjectType::People, ReadJson::sizes["People"], prev);
        putInCellGrid(p, ObjectType::People, ReadJson::sizes["People"]);
    }
    else if (prevCell->getGroundTransportation()) {
        target = prevCell->getGroundTransportation();
        if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Car) {
            prevCell->getTile()->subCar(1);
            nextCell->getTile()->addCar(1);
            amount = ReadJson::capacities[GroundTransportation::typeToString(GroundTransportationType::Car)];
            GroundTransportation* g = prevCell->getGroundTransportation();
            g->setLocation(next);
            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Car"], prev);
            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Car"]);
        }
        else if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Truck) {
            prevCell->getTile()->subTruck(1);
            nextCell->getTile()->addTruck(1);
            amount = ReadJson::capacities[GroundTransportation::typeToString(GroundTransportationType::Truck)];
            GroundTransportation* g = prevCell->getGroundTransportation();
            g->setLocation(next);
            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Truck"], prev);
            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Truck"]);
        }
        
    }
    else if (prevCell->getAirTransport()) {
        if (prevCell->getAirTransport()->getType() == AirTransportType::Helicopter) {
            prevCell->getTile()->subHelicopter(1);
            prevCell->getTile()->addHelicopter(1);
            amount = ReadJson::capacities[AirTransport::typeToString(AirTransportType::Helicopter)];
        }
        target = prevCell->getAirTransport();
        AirTransport* a = prevCell->getAirTransport();
        a->setLocation(next);
        putInCellGrid(ObjectType::AirTransport, ReadJson::sizes["Helicopter"], prev);
        putInCellGrid(a, ObjectType::AirTransport, ReadJson::sizes["Helicopter"]);
    }
    else return;
    for (auto &rs : resourses) {
        s->subResource(rs.first, min(amount[ReadJson::resourceTypes[rs.first]] - target->getResources()[rs.first], rs.second));
        target->addResource(rs.first, min(amount[ReadJson::resourceTypes[rs.first]] - target->getResources()[rs.first], rs.second));
    }
}
void World::rainFall(int amount)
{
    std::string tileName;
    std::string resourceType;
    for (int i = 0; i < tileGrid.size(); i++)
        for (int j = 0; j < tileGrid[0].size(); j++)
        {
            tileName = tileGrid[i][j]->getTile().first;
            if (ReadJson::tilesResourceType.find(tileName) != ReadJson::tilesResourceType.end()) {
                resourceType = ReadJson::tilesResourceType[tileName];
                if (ReadJson::rains.find(resourceType) != ReadJson::rains.end())
                    tileGrid[i][j]->addResource(resourceType, (amount + rainsLeft[resourceType]) / ReadJson::rains[resourceType]);
            }
        }
    for (auto rl : rainsLeft) {
        rl.second = (amount + rl.second) % ReadJson::rains[rl.first];
    }
}
//bool World::isGroundTransportation(std::pair<int, int> cell) {
//void World::move(std::pair<int, int> prev, std::pair<int, int> next) {
//    Coordinate* prevCell = getCell(prev);
//    Coordinate* nextCell = getCell(next);
//    
//    
//   
//    if (prevCell->getGroundTransportation()) {
//        if (isSettlement(prev))
//            return;
//        if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Car) {
//            prevCell->getTile()->subCar(1);
//            nextCell->getTile()->addCar(1);
//            GroundTransportation* g = prevCell->getGroundTransportation();
//            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Car"], prev);
//            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Car"]);
//        }
//
//        else if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Truck) {
//            prevCell->getTile()->subTruck(1);
//            nextCell->getTile()->addTruck(1);
//            GroundTransportation* g = prevCell->getGroundTransportation();
//            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Truck"], prev);
//            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Truck"]);
//        }
//
//
//    }
//    else if (prevCell->getAirTransport()) {
//        if (prevCell->getAirTransport()->getType() == AirTransportType::Helicopter) {
//            prevCell->getTile()->subHelicopter(1);
//            nextCell->getTile()->addHelicopter(1);
//
//        }
//        AirTransport* a = prevCell->getAirTransport();
//        putInCellGrid(ObjectType::AirTransport, ReadJson::sizes["Helicopter"], prev);
//        putInCellGrid(a, ObjectType::AirTransport, ReadJson::sizes["Helicopter"]);
//    }
//}

//std::pair<int,int> World::move(std::pair<int, int> prev, std::pair<int, int> next) {
//    Coordinate* prevCell = getCell(prev);
//    Coordinate* nextCell = getCell(next);
//    if (prevCell->getAirTransport()) {
//        if (nextCell->getAirTransport())
//            next.first = next.first - ReadJson::sizes[AirTransport::typeToString(prevCell->getAirTransport()->getType())][0];
//        if (prevCell->getAirTransport()->getType() == AirTransportType::Helicopter) {
//            prevCell->getTile()->subHelicopter(1);
//            nextCell->getTile()->addHelicopter(1);
//        }
//        AirTransport* a = prevCell->getAirTransport();
//        a->setLocation(next);
//        putInCellGrid(ObjectType::AirTransport, ReadJson::sizes["Helicopter"], a->getLocation());
//        putInCellGrid(a, ObjectType::AirTransport, ReadJson::sizes["Helicopter"]);
//        return next;
//    }
//    ///אם היעד תפוס על ידי כלי רכב אחר
//    
//    //Road* r = isRoad(next);
//   /* if (!r)
//        return ;*/
//    if (prevCell->getGroundTransportation()) {
//        if (nextCell->getGroundTransportation() || nextCell->getPeople())
//        {
//            //לא בדקתי בדיקות רציניות רק שמתי את ברכב מאחוריו ללא אפשרות לצידו ניתן כמובן להרחיב
//            next.first -= ReadJson::sizes[GroundTransportation::typeToString(nextCell->getGroundTransportation()->getType())][0];
//        }
//        if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Car) {
//            prevCell->getTile()->subCar(1);
//            nextCell->getTile()->addCar(1);
//            GroundTransportation* g = prevCell->getGroundTransportation();
//            g->setLocation(next);
//            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Car"], g->getLocation());
//            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Car"]);
//        }
//        else if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Truck) {
//            prevCell->getTile()->subTruck(1);
//            nextCell->getTile()->addTruck(1);
//            GroundTransportation* g = prevCell->getGroundTransportation();
//            g->setLocation(next);
//            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Truck"], g->getLocation());
//            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Truck"]);
//        }
//        return next;
//    }
//    if (prevCell->getPeople()) {
//        if (nextCell->getPeople())
//            next.first = next.first - ReadJson::sizes["People"][0];
//        prevCell->getTile()->subPeople(1);
//        nextCell->getTile()->addPeople(1);
//        
//        People* p = prevCell->getPeople();
//        p->setLocation(next);
//        putInCellGrid(ObjectType::People, ReadJson::sizes["People"], p->getLocation());
//        putInCellGrid(p, ObjectType::People, ReadJson::sizes["People"]);
//        return next;
//    }
//    
//    return prev;
//}

std::pair<int, int> World::move(std::pair<int, int> prev, std::pair<int, int> next) {
    Coordinate* prevCell = getCell(prev);
    Coordinate* nextCell = getCell(next);
    Settlement* prevS = isSettlement(prev);
    Settlement* nextS = isSettlement(next);
    bool occupied;
    if (nextCell->getAirTransport() || nextCell->getGroundTransportation() || nextCell->getPeople())
        occupied = true;
    if (prevCell->getAirTransport()) {
        if (occupied)
            next.first = next.first - ReadJson::sizes[AirTransport::typeToString(prevCell->getAirTransport()->getType())][0];
        if (prevCell->getAirTransport()->getType() == AirTransportType::Helicopter) {
            prevCell->getTile()->subHelicopter(1);
            nextCell->getTile()->addHelicopter(1);
        }
        AirTransport* a = prevCell->getAirTransport();
        putInCellGrid(ObjectType::AirTransport, ReadJson::sizes["Helicopter"], a->getLocation());
        a->setLocation(next);
        putInCellGrid(a, ObjectType::AirTransport, ReadJson::sizes["Helicopter"]);
        return next;
    }
    if (prevCell->getGroundTransportation()) {
        if (occupied)
            next.first = next.first - ReadJson::sizes[GroundTransportation::typeToString(prevCell->getGroundTransportation()->getType())][0];
        if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Car) {
            prevCell->getTile()->subCar(1);
            nextCell->getTile()->addCar(1);
            GroundTransportation* g = prevCell->getGroundTransportation();
            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Car"], g->getLocation());
            g->setLocation(next);
            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Car"]);
        }
        else if (prevCell->getGroundTransportation()->getType() == GroundTransportationType::Truck) {
            prevCell->getTile()->subTruck(1);
            nextCell->getTile()->addTruck(1);
            GroundTransportation* g = prevCell->getGroundTransportation();
            putInCellGrid(ObjectType::GroundTransportation, ReadJson::sizes["Truck"], g->getLocation());
            g->setLocation(next);
            putInCellGrid(g, ObjectType::GroundTransportation, ReadJson::sizes["Truck"]);
        }
        return next;
    }
    if (prevCell->getPeople() && nextCell->getTile()->getTile().first != "Water") {
        if (occupied)
            next.first = next.first - ReadJson::sizes["People"][0];
        if (prevS)
            prevS->addPeople(1);
        else
            prevCell->getTile()->subPeople(1);
        if (nextS)
            nextS->addPeople(1);
        else
            nextCell->getTile()->addPeople(1);
        People* p = prevCell->getPeople();
        putInCellGrid(ObjectType::People, ReadJson::sizes["People"], p->getLocation());
        p->setLocation(next);
        putInCellGrid(p, ObjectType::People, ReadJson::sizes["People"]);
        return next;
    }
    return prev;
}


//    if (cellGrid[cell.second - 1][cell.first - 1]->getGrou()) {
//        Settlement* s = dynamic_cast<Settlement*>(cellGrid[cell.second - 1][cell.first - 1]->getGroundObject());
//        if (s) {
//            return s;
//        }
//    }
//    return nullptr;
//}

//asserts
std::string World::selectedComplete(std::pair<int, int> cell)
{
    return getCell(cell)->getGroundObject()->isComplete() ? "true" : "false";
}
std::string World::selectedCategory(std::pair<int, int> cell)
{
    if (getCell(cell)->getPeople())
        return "People";
    if (getCell(cell)->getGroundObject())
        return GroundObject::typeToString(getCell(cell)->getGroundObject()->getType());
    return  getCell(cell)->getTile()->getTile().first;
}
std::vector<int> World::selectedResource(std::pair<int, int>cell)
{
    Coordinate* currentCell = getCell(cell);
    Settlement* s = isSettlement(cell);
    if (s)
        return s->selectedResource();
    if (currentCell->getPeople())
        return currentCell->getPeople()->selectedResource();
    if (currentCell->getGroundTransportation())
        return currentCell->getGroundTransportation()->selectedResource();
    if (currentCell->getAirTransport())
        return currentCell->getAirTransport()->selectedResource();
    return currentCell->getTile()->selectedResource();
}
std::string World::selectedCoordinates(std::pair<int, int> cell) {
    return "7 3";
}
int World::selectedPeople(std::pair<int, int>cell)
{
    Settlement* s = isSettlement(cell);
    if (s)
        return s->getPeoples();
    return getCell(cell)->getTile()->getPeoples();
}
int World::selectedCar(std::pair<int, int>cell)
{
    Settlement* s = isSettlement(cell);
    if (s)
        return s->getCars();
    return getCell(cell)->getTile()->getCars();
}
int World::selectedTruck(std::pair<int, int>cell)
{
    Settlement* s = isSettlement(cell);
    if (s)
        return s->getTrucks();
    return getCell(cell)->getTile()->getTrucks();
}
int World::cityCount()
{
    return Settlement::getCityCounter();
}
int World::villageCount()
{
    return Settlement::getVillageCounter();
}
int World::roadCount()
{
    return Road::getRoadCounter();
}
int World::carCount()
{
    return GroundTransportation::getCarCounter();
}
int World::truckCount()
{
    return GroundTransportation::getTruckCounter();
}
int World::helicopterCount()
{
    return AirTransport::getHelicopterCounter();
}

bool World::putInCellGrid(Location* location,ObjectType type, std::vector<int>size) {

     for (int i = location->getLocation().second; i < location->getLocation().second + size[1]; i++)
        for (int j = location->getLocation().first; j < location->getLocation().first + size[0]; j++)
            cellGrid[i][j]->setObject(type,location);
     return true;
}
bool World::putInCellGrid(ObjectType type, std::vector<int>size,std::pair<int,int> cell) {

    for (int i = cell.second; i < cell.second + size[1]; i++)
        for (int j = cell.first; j < cell.first + size[0]; j++)
            cellGrid[i][j]->setObject(type, nullptr);
    return true;
}
