#include "ReadImages.h"
#include "ReadJson.h"
#include "Moduls.h"

std::string ReadImages::path= "..\\assets\\";
std::unordered_map <int, cv::Mat> ReadImages::tiles;
int ReadImages::scaleFactor = 20;
cv::Mat ReadImages::road;
std::unordered_map <std::string, cv::Mat>ReadImages::groundObjects;
std::unordered_map <std::string, cv::Mat>ReadImages::transportation;



std::unordered_map<int, cv::Mat>& ReadImages::getTiles()
{
    static std::unordered_map<int, cv::Mat> images;
    // קריאת כל התמונות מהתיקייה
    for (auto& t : ReadJson::tiles) {

        std::string imagePath = path + "Tiles\\" + t.second + ".png";
        cv::Mat image = cv::imread(imagePath);

        if (!image.empty()) {
            cv::resize(image, image, cv::Size(ReadJson::sizes["Tile"][0] * scaleFactor, ReadJson::sizes["Tile"][1] * scaleFactor));
            images[t.first] = image.clone();
        }
        else
            std::cerr << "Unable to load image: " << imagePath << std::endl;
    }
	return images;
}

cv::Mat& ReadImages::getRoad()
{
    static cv::Mat image;
    std::string imagePath = path + "Tiles\\" + "Road" + ".png";
    image = cv::imread(imagePath);

    if (!image.empty())
        cv::resize(image, image, cv::Size(ReadJson::sizes["Road"][0] * scaleFactor, ReadJson::sizes["Road"][1] * scaleFactor));
    else
        std::cerr << "Unable to load image: " << imagePath << std::endl;
    return image;
}

std::unordered_map<string, cv::Mat>& ReadImages::getGroundObjects()
{
    static std::unordered_map<std::string, cv::Mat> images;
    
    std::vector<std::string> types={ "Road","City","Village" };

    for (auto &t : types) {
        std::string imagePath = path + "GroundObjects\\" + t + ".png";
        cv::Mat image = cv::imread(imagePath);
        if (!image.empty()) {
            cv::resize(image, image, cv::Size(ReadJson::sizes[t][0] * scaleFactor, ReadJson::sizes[t][1] * scaleFactor));
            images[t] = image.clone();
        }
        else
            std::cerr << "Unable to load image: " << imagePath << std::endl;
    }
    return images;
}

std::unordered_map<string, cv::Mat>& ReadImages::getTransportation()
{
    static std::unordered_map<std::string, cv::Mat> images;

    std::vector<std::string> types = { "Helicopter","Car","Truck" };

    for (auto& t : types) {
        std::string imagePath = path + "Transposation\\" + t + ".png";
        cv::Mat image = cv::imread(imagePath);
        if (!image.empty()) {
            cv::resize(image, image, cv::Size(ReadJson::sizes[t][0] * scaleFactor, ReadJson::sizes[t][1] * scaleFactor));
            images[t] = image.clone();
        }
        else
            std::cerr << "Unable to load image: " << imagePath << std::endl;
        cv::imwrite(imagePath, image);

    }
    return images;
}

void ReadImages::init()
{
	tiles=getTiles();
    road = getRoad();
    groundObjects = getGroundObjects();
    transportation = getTransportation();
}

