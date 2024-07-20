#ifdef _DEBUG
#pragma comment(lib, "opencv_world4100d.lib")
#else
#pragma comment(lib, "opencv_world4100.lib")
#endif

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include "World.h"
#include "Input.h"
#include "ReadJson.h"
#include <unordered_map>
#include"Game.h"

using namespace std;
int main()
{
    



	ReadJson::init();
	ReadImages::init();

	Game game;
}
