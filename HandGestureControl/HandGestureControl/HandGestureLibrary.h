#pragma once
#include "Colors.h"

class HandGestureLibrary
{
public:
	HandGestureLibrary();
	~HandGestureLibrary();

	bool Initalize(Colors firstColor, Colors secondColor, Mat framesThresholdRED, Mat framesThresholdCOLOR, Mat &framesOriginal, const string &windowNameGaussThresh, const int objectArea_min, const int objectArea_max);
	void WaveRight(Colors firstColor, Colors secondColor, Mat framesThresholdRED, Mat framesThresholdCOLOR, Mat &framesOriginal, const string &windowNameGaussThresh, const int objectArea_min, const int objectArea_max);


};

