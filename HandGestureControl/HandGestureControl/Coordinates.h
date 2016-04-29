#pragma once
#include <vector>
#include "Colors.h"

class Coordinates
{
public:
	Coordinates();
	~Coordinates();


	void getCoords(Colors firstColor, Colors secondColor, Mat framesThresholdRED, Mat framesThresholdBLUE, Mat &framesOriginal, const string &windowNameGaussThresh, const int objectArea_min, const int objectArea_max);

};

