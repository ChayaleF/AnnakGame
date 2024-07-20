#pragma once
#include "Input.h"
#include "World.h"
#include "Tile.h"
#include "People.h"
#include "GameVisual.h"
class Game
{
private:
	int points;
	Input input;
	World world;
	GameVisual gameVisual;
	std::pair<int,int> currentTile;
	std::pair<int,int> moveTile;

public:
	Game():moveTile(pair<int,int>(0,0)), input("input.txt", "output.txt"), world(), gameVisual() {
		points = 0;
		input.parse_and_store();
		initworld();
		gameVisual.initByTiles(world.getTileGrid());
		start();
		gameVisual.show();
		inputSteps();
		asserts();

	}
	void play() {

	}
	void initworld() {
		world.fillTileGrid(input.world->data);
		world.fillCellGrid();
	}
	void start() {
		for (auto& a : input.start)
		{
			if (a.get()->name == Command::RESOURCE)
				resource(a.get());
			else if (a.get()->name == Command::PEOPLE)
				people(a.get());
			else if (a.get()->name == Command::BUILD)
				buildStart(a.get());
			else if (a.get()->name == Command::MAKE_EMPTY)
				makeEmpty(a.get());
			else if (a.get()->name == Command::MANUFACTURE)
				manufactureStart(a.get());
			else if (a.get()->name == Command::RESOURCES)
				resources(a.get());
			else if (a.get()->name == Command::SET_POINTS)
				SetPointsStart(a.get());
		}	
	}
	void inputSteps() {
		for (auto a:input.steps)
		{
			if (a.get()->name == Command::SELECT)
				select(a.get());
			else if (a.get()->name == Command::RAIN)
				rain(a.get());
			else if (a.get()->name == Command::WAIT)
				wait(a.get());
			else if (a.get()->name == Command::WORK)
				work(a.get());
			else if (a.get()->name == Command::BUILD)
				buildInput(a.get());
			else if (a.get()->name == Command::PEOPLE)
				people(a.get());
			else if (a.get()->name == Command::TAKE_RESOURCES)
				takeResourse(a.get());
			else if (a.get()->name == Command::DEPOSIT)
				deposit(a.get());
			else if (a.get()->name == Command::MANUFACTURE)
				manufactureInput(a.get());
			else if (a.get()->name == Command::MOVE)
				moveIn(a.get());
			else if (a.get()->name == Command::RESOURCES)
				resources(a.get());

		}
	}
	void asserts() {

		for (auto a : input.asserts)
		{
			if (a == "SelectedResource")
				selectedResource();
			else if (a == "SelectedCategory")
				selectedCategory();
			else if (a == "SelectedComplete")
				selectedComplete();
			else if (a == "SelectedCoordinates")
				selectedCoordinates();
			else if (a == "SelectedPeople")
				selectedPeople();
			else if (a == "SelectedCar")
				selectedCar();
			else if (a == "SelectedTruck")
				selectedTruck();
			else if (a == "CityCount")
				cityCount();
			else if (a == "VillageCount")
				villageCount();
			else if (a == "RoadCount")
				roadCount();
			else if (a == "CarCount")
				carCount();
			else if (a == "TruckCount")
				truckCount();
			else if (a == "HelicopterCount")
				helicopterCount();
			else if (a == "Points")
				pointsAssert();
			
		}
	}
	void SetPoints(int p) {
		points = std::min(100, points + p);
	}
	//start
	void resource(Command* command) {
		world.insertResource(std::stoi(command->arguments[0]), command->arguments[1], std::stoi(command->arguments[2]), std::stoi(command->arguments[3]));
	}
	void people(Command* command) {

		world.insertPeople(std::stoi(command->arguments[0]), std::stoi(command->arguments[1]), std::stoi(command->arguments[2]));
	}
	void buildStart(Command* command) {
		SetPoints(world.buildGroundObject(command->arguments[0], std::stoi(command->arguments[1]), std::stoi(command->arguments[2]),true));
		gameVisual.buildGroundObjects(command->arguments[0],std::stoi(command->arguments[1])-1, std::stoi(command->arguments[2])-1);
	}
	void makeEmpty(Command* command) {
		world.makeEmpty(std::stoi(command->arguments[0]), std::stoi(command->arguments[1]));
	}
	void manufactureStart(Command* command) {
		world.manufacture(command->arguments[0], std::stoi(command->arguments[1]), std::stoi(command->arguments[2]),true);
		gameVisual.buildTransposation(command->arguments[0], std::stoi(command->arguments[1]) - 1, std::stoi(command->arguments[2]) - 1);
	}
	void resources(Command* command) {
		world.insertResources(std::vector<int> { std::stoi(command->arguments[0]), std::stoi(command->arguments[1]), std::stoi(command->arguments[2]), std::stoi(command->arguments[3]) }, std::stoi(command->arguments[4]), std::stoi(command->arguments[5]));
	}
	void SetPointsStart(Command* command) {
		SetPoints(std::stoi(command->arguments[0]));
	}

	//input
	void select(Command* command) {
		currentTile = std::pair<int, int>(std::stoi(command->arguments[0]), std::stoi(command->arguments[1]));
	}
	void buildInput(Command* command) {
		points += world.buildGroundObject(command->arguments[0], std::stoi(command->arguments[1]), std::stoi(command->arguments[2]),false);
	}
	void wait(Command* command) {
		for (size_t i = 0; i <std::stoi( command->arguments[0]); i++);
	}
	void work(Command* command) {
		world.peopleWork(currentTile,std::pair<int,int>(std::stoi(command->arguments[0]), std::stoi(command->arguments[1])));
	}
	void rain(Command* command) {
		world.rainFall(std::stoi(command->arguments[0]));
	}
	void takeResourse(Command* command) {
		world.takeResources(currentTile,std::pair<int, int>(std::stoi(command->arguments[0]), std::stoi(command->arguments[1])));
	}
	void deposit(Command* command) {
		world.deposit(currentTile, std::pair<int, int>(std::stoi(command->arguments[0]), std::stoi(command->arguments[1])));
	}
	void manufactureInput(Command* command) {
		world.manufacture(command->arguments[0], std::stoi(command->arguments[1]), std::stoi(command->arguments[2]),false);
	}
	void moveIn(Command* command) {
		moveTile= world.move(currentTile, std::pair<int, int>(std::stoi(command->arguments[0]), std::stoi(command->arguments[1])));
	}

	//asserts
	void selectedResource() {
		std::cout << "SelectedResource ";
		for (int a : world.selectedResource(currentTile))
			std::cout << a << " ";
		cout << endl;
	}
	void selectedCategory() {
		string resource = world.selectedCategory(currentTile);
		std::cout << "SelectedCategory " << resource<<endl;
	}
	void selectedComplete() {
		cout << "SelectedComplete " << world.selectedComplete(currentTile)<<endl;
	}
	void selectedPeople() {
		cout << "SelectedPeople " << world.selectedPeople(currentTile) << endl;
	}
	void selectedCar() {
		cout << "SelectedCar " << world.selectedCar(currentTile) << endl;
	}
	void selectedTruck() {
		cout << "SelectedTruck " << world.selectedTruck(currentTile) << endl;
	}
	void selectedCoordinates() {
		cout << "SelectedCoordinates " << moveTile.first<<" "<< moveTile.second << endl;
	}
	
	void cityCount() {
		cout << "CityCount " << world.cityCount()<<endl;
	}
	void villageCount() {
		cout << "VillageCount " << world.villageCount()<<endl;
	}
	void roadCount() {
		cout << "RoadCount " << world.roadCount()<<endl;
	}
	void carCount() {
		cout << "CarCount " << world.carCount() << endl;
	}
	void truckCount() {
		cout << "TruckCount " << world.truckCount() << endl;
	}
	void helicopterCount() {
		cout << "HelicopterCount " << world.helicopterCount() << endl;
	}
	void pointsAssert() {
		cout << "Points " << points << endl;
	}
};

