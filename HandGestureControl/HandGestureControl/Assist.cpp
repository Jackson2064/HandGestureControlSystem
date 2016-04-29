#include "Assist.h"
#include "DrawCenters.h"
#include "trackColor.h"
#include "Colors.h"
#include "Coordinates.h"
#include "handDistance.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2/opencv.hpp> //Contains: VideoCapture
using namespace cv; // In std: VideoCapture
using namespace std;


Assist::Assist()
{
}


Assist::~Assist()
{
}

//int Assist::Init_Gesture(bool start, int count, Mat &framesOriginal)
//{
//	if (start == true)
//	{
//		int count = count + 1;
//		return count;
//	}
//	else
//	{ // it's false
//		int count = 0;
//		return count;
//
//	}
//	if ( count > 20)
//	{
//		putText(framesOriginal, "START GESTURES", Point(30, 230), 1, 1, Scalar(0, 255, 0), 2);
//	}
//}