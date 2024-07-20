#ifdef _DEBUG
#pragma comment(lib, "opencv_world4100d.lib")
#else
#pragma comment(lib, "opencv_world4100.lib")
#endif
#pragma once
#include <opencv2/opencv.hpp>
#include<string>
#include <unordered_map>
class ReadImages
{
private:
	static std::string path;
	static std::unordered_map <int, cv::Mat>& getTiles();
	static cv::Mat& getRoad();
	static std::unordered_map <std::string, cv::Mat>& getGroundObjects();
	static std::unordered_map <std::string, cv::Mat>& getTransportation();


public:
	static int scaleFactor;
	static void init();
	static std::unordered_map <int, cv::Mat> tiles;
	static cv::Mat road;
	static std::unordered_map <std::string, cv::Mat> groundObjects;
	static std::unordered_map <std::string, cv::Mat> transportation;
		


};

