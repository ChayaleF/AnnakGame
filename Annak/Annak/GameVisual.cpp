#include "GameVisual.h"


void GameVisual::drawGrid(cv::Mat& image, int cellSizeRow, int cellSizeCol, std::vector<int> gridDivisions)
{
    int rows = image.rows / cellSizeRow;
    int cols = image.cols / cellSizeCol;
    // ציור הרשת הראשית
    for (int i = 0; i <= rows; ++i) {
        cv::line(image, cv::Point(0, i * cellSizeRow), cv::Point(image.cols, i * cellSizeRow), cv::Scalar(200, 200, 200), 2);
    }
    for (int j = 0; j <= cols; ++j) {
        cv::line(image, cv::Point(j * cellSizeCol, 0), cv::Point(j * cellSizeCol, image.rows), cv::Scalar(200, 200, 200), 2);
    }
    // ציור הרשת המורחבת של ריבועים קטנים
    int smallCellSizeHeight = cellSizeRow / gridDivisions[1];
    int smallCellSizeWidth = cellSizeCol / gridDivisions[0];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 1; k < gridDivisions[1]; ++k) {
                cv::line(image, cv::Point(j * cellSizeCol, i * cellSizeRow + k * smallCellSizeHeight),
                    cv::Point((j + 1) * cellSizeCol, i * cellSizeRow + k * smallCellSizeHeight), cv::Scalar(200, 200, 200), 1);
            }
            for (int k = 1; k < gridDivisions[0]; ++k) {
                cv::line(image, cv::Point(j * cellSizeCol + k * smallCellSizeWidth, i * cellSizeRow),
                    cv::Point(j * cellSizeCol + k * smallCellSizeWidth, (i + 1) * cellSizeRow), cv::Scalar(200, 200, 200), 1);
            }
        }
    }
}

void GameVisual::initByTiles(std::vector<std::vector<Tile*>> worldMap)
{
    int mapWidth = worldMap[0].size();
    int mapHeight = worldMap.size();
    cv::Mat mapImage(mapHeight * ReadImages::tiles[1].rows, mapWidth * ReadImages::tiles[1].cols, CV_8UC3, cv::Scalar(255, 255, 255));
    //int cellSize = images[1].rows; // יש להשתמש בגודל תמונה כדי שהרשת יהיה עם גודל המתאים למפה
    vector<int> gridDivisions = ReadJson::sizes["Tile"];
    // ציור המפה על התמונה
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            int tileType = worldMap[i][j]->getTile().second;
            cv::Mat tile = ReadImages::tiles[tileType];
            cv::Rect roi(j * tile.cols, i * tile.rows, tile.cols, tile.rows);
            tile.copyTo(mapImage(roi));
        }
    }
    // ציור הרשת על התמונה
    drawGrid(mapImage, ReadImages::tiles[1].rows, ReadImages::tiles[1].cols, gridDivisions);
    imageWorldMap = mapImage;
}


void GameVisual::buildGroundObjects(std::string type, int x, int y)
{
    cout << endl << "groundObjects" << endl;
    cv::Mat typeImage = ReadImages::groundObjects[type];
    
    cv::Rect roi(x * ReadImages::scaleFactor, y * ReadImages::scaleFactor, typeImage.cols, typeImage.rows);
    std::cout << " roi.x: " << roi.x << "roi.y: " << roi.y << endl;
    std::cout << " roi.width: " << roi.width << "roi.height: " << roi.height << endl;
    std::cout << " col: " << imageWorldMap.cols << "row " << imageWorldMap.rows << endl;

    if (roi.x + roi.width > imageWorldMap.cols || roi.y + roi.height > imageWorldMap.rows)
    {
        std::cerr << "Error: ROI is out of bounds." << std::endl;
        return;
    }
    typeImage.copyTo(imageWorldMap(roi));   
}
void GameVisual::buildTransposation(std::string type, int x, int y)
{
    cout << endl << "Transposation" << endl;
    cv::Mat typeImage = ReadImages::transportation[type];
    if (typeImage.empty()) {
        std::cerr << "Error: Could not load image" << std::endl;
        return ;
    }


    cv::Rect roi(x * ReadImages::scaleFactor, y * ReadImages::scaleFactor, typeImage.cols, typeImage.rows);
    std::cout << " roi.x: " << roi.x << "roi.y: " << roi.y << endl;
    std::cout << " roi.width: " << roi.width << "roi.height: " << roi.height << endl;
    std::cout << " col: " << imageWorldMap.cols << "row " << imageWorldMap.rows << endl;
    if (roi.x + roi.width > imageWorldMap.cols || roi.y + roi.height > imageWorldMap.rows)
    {
        std::cerr << "Error: ROI is out of bounds." << std::endl;
        return;
    }
    typeImage.copyTo(imageWorldMap(roi));
    std::cout << "Error:" << std::endl;

}

void GameVisual::show()
{
    cv::imshow("World Map", imageWorldMap);
    cv::waitKey(0);
}
