#pragma once
#include "Colors.h"

class Wave
{
public:
	Wave();
	~Wave();

	bool WaveIn(Colors firstColor, Colors secondColor, Mat framesThresholdRED, Mat framesThresholdCOLOR, Mat &framesOriginal, const string &windowNameGaussThresh, const int objectArea_min, const int objectArea_max);
	int WaveOut(Colors firstColor, Colors secondColor, Mat framesThresholdRED, Mat framesThresholdCOLOR, Mat &framesOriginal, const string &windowNameGaussThresh, const int objectArea_min, const int objectArea_max);

};

