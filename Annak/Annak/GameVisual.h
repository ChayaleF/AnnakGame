#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "Tile.h"
#include "ReadImages.h"
class GameVisual {
private:

    cv::Mat imageWorldMap;
    void drawGrid(cv::Mat& image, int cellSizeRow, int cellSizeCol, std::vector<int> gridDivisions);


public:  
    GameVisual(){}
    void initByTiles(std::vector<std::vector<Tile*>> worldMap);
    void buildGroundObjects(std::string type, int x, int y);
    void buildTransposation(std::string type, int x, int y);
    void show();
};