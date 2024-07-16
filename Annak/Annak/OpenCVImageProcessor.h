#ifdef _DEBUG
#pragma comment(lib, "opencv_world4100d.lib")
#else
#pragma comment(lib, "opencv_world4100.lib")
#endif
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "Tile.h"
class OpenCVImageProcessor {
private:
    std::vector<std::vector<Tile*>> worldMap;
    std::map<int, cv::Mat> images;

    std::map<int, cv::Mat> loadImages(const std::string& folderPath) {
        int scaleFactor = 20;
        std::map<int, cv::Mat> images;
        // קריאת כל התמונות מהתיקייה
        for (auto& t : ReadJson::tiles) {
            std::string imagePath = folderPath + "\\" + t.second + ".png";
            cv::Mat image = cv::imread(imagePath);
            if (!image.empty())
            {
                cout << ReadJson::sizes["Tile"][1];
                cv::resize(image, image, cv::Size(ReadJson::sizes["Tile"][0] * scaleFactor, ReadJson::sizes["Tile"][1] * scaleFactor));
                images[t.first] = image.clone();
            }
            // שמירה של תמונה מקורית במפה
            else
                std::cerr << "Unable to load image: " << imagePath << std::endl;
        }
        return images;
    }

    
    void drawGrid(cv::Mat& image, int cellSizeRow, int cellSizeCol, std::vector<int> gridDivisions) {
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


public:  
    OpenCVImageProcessor(){}
    OpenCVImageProcessor(std::vector<std::vector<Tile*>> worldMap, const std::string& imagePath) : worldMap(worldMap), images(loadImages(imagePath)) {
        std::cout << imagePath << endl;
    }
    void processImages() {
        int mapWidth = worldMap[0].size();
        int mapHeight = worldMap.size();
        cv::Mat mapImage(mapHeight * images[1].rows, mapWidth * images[1].cols, CV_8UC3, cv::Scalar(255, 255, 255));
        //int cellSize = images[1].rows; // יש להשתמש בגודל תמונה כדי שהרשת יהיה עם גודל המתאים למפה
        vector<int> gridDivisions= ReadJson::sizes["Tile"];
        // ציור המפה על התמונה
        for (int i = 0; i < mapHeight; ++i) {
            for (int j = 0; j < mapWidth; ++j) {
                int tileType = worldMap[i][j]->getTile().second;
                cv::Mat tile = images[tileType];
                cv::Rect roi(j * tile.cols, i * tile.rows, tile.cols, tile.rows);
                tile.copyTo(mapImage(roi));
            }
        }
        // ציור הרשת על התמונה
        drawGrid(mapImage, images[1].rows, images[1].cols, gridDivisions);
        // הצגת המפה בחלון
        cv::imshow("World Map", mapImage);
        cv::waitKey(0);
    }
    void setWorldMap(const std::vector<std::vector<Tile*>>& newWorldMap) {
        worldMap = newWorldMap;
    }
    void setImages(const std::string& imagePath) {
        images = loadImages(imagePath);
    }
};