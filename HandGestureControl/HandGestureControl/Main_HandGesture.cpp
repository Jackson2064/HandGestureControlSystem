//Detect and track a single color
//Open a RGB, HSV, and Threshold window
//Treshold window gives binary black(=0) and white(=1)
//Open up Trackbar to change HSV values to isolate selected color
//Isolating color will help give us values for computation

#define _CRT_SECURE_NO_DEPRECATE

#include "removeNoise.h"
#include "Wave.h"
#include "trackColor.h"
#include "Colors.h"
#include "handDistance.h"
#include "Coordinates.h"
#include "HandGestureLibrary.h"
#include <sstream>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2/opencv.hpp> //Contains: VideoCapture
#include <string>
#include <vector>


using namespace cv; // In std: VideoCapture,
using namespace std; // In std: <string>, 

//INITALIZE 

//window width and height
const int Width = 640;
const int Height = 480;

//Names of the windows that will pop up and trackbar
const string windowNameOriginal = "Original Window";
const string windowNameHSV = "HSV Window";
const string windowNameThreshold = "Threshold Window";
const string windowNameTrackbars = "HSV Trackbars";
const string windowNameMorphTrans = "Morphological Transformations";
const string windowNameGaussThresh = "Gaussian Blur of Threshold";

//HSV trackbar min and max values
int H_min = 0;
int H_max = 256;
int S_min = 0;
int S_max = 256;
int V_min = 0;
int V_max = 256;

//min and max area of object being detected
//objects smaller than 25x25 pixels will be considered noise
//objects bigger than 3/4 of the screen probably just mean the HSV values need to be adjusted to make filter better
const int objectArea_min = 15 * 15;
const int objectArea_max = Width*Height*(3 / 4);



//FUNCTIONS

//call this function when track position is changed
void rangeControl(int, void*)
{}

//function to create Trackbar window
void Trackbars()
{
	char trackbarLabels[25]; //var for trackbar labels, max 25 characters in length

	// make the trackbar window
	namedWindow(windowNameTrackbars, 0);  // Userdata=0, otherwise autosize

	// now create trackbars for the window created
	//&var means you are passing address of variable (pointer)
	//createTrackbar(trackbarname, winname, value, count, OnChange)
	//trackbarname: name of the created trackbar
	//winname: name of parent window for trackbar
	//value: optional pointer to an integer variable whose value reflects the position of the slider
	//       slider position defined by variable
	//count: max position of slider
	//OnChange: Pointer to function to be called every time slider changes position
	createTrackbar("Hue Min", windowNameTrackbars, &H_min, H_max, rangeControl);
	createTrackbar("Hue Max", windowNameTrackbars, &H_max, H_max, rangeControl);
	createTrackbar("Sat Min", windowNameTrackbars, &S_min, S_max, rangeControl);
	createTrackbar("Sat Max", windowNameTrackbars, &S_max, S_max, rangeControl);
	createTrackbar("Value Min", windowNameTrackbars, &V_min, S_max, rangeControl);
	createTrackbar("Value Max", windowNameTrackbars, &V_max, S_max, rangeControl);

	// sprintf part of C library and sets labels for the trackbars
	//variable, label, value
	sprintf(trackbarLabels, "Hue Min", H_min);
	sprintf(trackbarLabels, "Hue Max", H_max);
	sprintf(trackbarLabels, "Sat Min", S_min);
	sprintf(trackbarLabels, "Sat Max", S_max);
	sprintf(trackbarLabels, "Value Min", V_min);
	sprintf(trackbarLabels, "Value Max", V_max);
	
}

int main()
{

	//int argc, char** argv
	bool calibrationMode = false;

	//Store Original frames in matrix
	Mat framesOriginal;

	//Store HSV frames
	Mat framesHSV;

	//Store Threshold  
	Mat framesThresh;
	Mat framesThreshRED, framesThreshORANGE, framesThreshYELLOW, framesThreshGREEN, framesThreshBLUE;

	//Store Gaussien Threshold frames
	Mat framesGauss;

	//create Videocapture variable for webcam
	VideoCapture capture;

	//Initalize counter for StartGestures
	int count = 0;

	//Open Trackbars
	if (calibrationMode)
	{
		Trackbars();
	}

	//Open up the default camera at location 0, location 1 would be...
	capture.open(0);

	//VideoCapture frame dimensions
	capture.set(CAP_PROP_FRAME_WIDTH, Width);
	capture.set(CAP_PROP_FRAME_HEIGHT, Height);
	bool BEGIN = false;
	bool wavein = false;
	int waveout = 0;
	bool staytrue = false;

		//Copy frames into matrix
		while (true)
		{
			//reads video frame by frame
			capture.read(framesOriginal);

			//cvtColor(framesOriginal, framesHSV, COLOR_BGR2HSV); // do i even need this?

			Colors red("red"), blue("blue"), green("green"), yellow("yellow"), orange("orange");
			remove_Noise remove;
			track trackColor;
			Coordinates Coords;
			HandGestureLibrary Gesture;
			Wave Waving;
			bool START;
			//int in, out; 
			

			if (calibrationMode == true)
			{
				//in calibration mode we track objects based on HSV slider values
				cvtColor(framesOriginal, framesHSV, COLOR_BGR2HSV);

				//HSV to Threshold
				inRange(framesHSV, Scalar(H_min, S_min, V_min), Scalar(H_max, S_max, V_max), framesThresh);

				remove_Noise remove;
				remove.noise(framesThresh);

				track trackColor;
				trackColor.track_Color(framesThresh, framesOriginal, objectArea_min, objectArea_max);

				imshow(windowNameThreshold, framesThresh);


			}
			else
			{
				
				//first find the reds
				cvtColor(framesOriginal, framesHSV, COLOR_BGR2HSV);
				//inRange(framesHSV, red.getminHSV(), red.getmaxHSV(), framesThresh);
				//remove.noise(framesThresh);
				//trackColor.track_Color(red, framesThresh, framesHSV, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);

				inRange(framesHSV, red.getminHSV(), red.getmaxHSV(), framesThreshRED);
				inRange(framesHSV, orange.getminHSV(), orange.getmaxHSV(), framesThreshORANGE);
				inRange(framesHSV, yellow.getminHSV(), yellow.getmaxHSV(), framesThreshYELLOW);
				inRange(framesHSV, green.getminHSV(), green.getmaxHSV(), framesThreshGREEN);
				inRange(framesHSV, blue.getminHSV(), blue.getmaxHSV(), framesThreshBLUE);

				remove.noise(framesThreshRED, red);
				remove.noise(framesThreshORANGE);
				remove.noise(framesThreshYELLOW);
				remove.noise(framesThreshGREEN);
				remove.noise(framesThreshBLUE);

				
				//get_Coords.X_Coord(red, orange, framesThreshRED, framesThreshORANGE, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);
				//get_Coords.X_Coord(red, yellow, framesThreshRED, framesThreshYELLOW, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);
				Coords.getCoords(red, green, framesThreshRED, framesThreshGREEN, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);
				//get_Coords.X_Coord(red, blue, framesThreshRED, framesThreshBLUE, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);
				
				
				START=Gesture.Initalize(red, green, framesThreshRED, framesThreshGREEN, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);
				
				//bool START = false;
				if (START==true)
				{
					count = count + 1;
					
				}
				else
				{ // it's false
					count = 0;

				}
				if (count > 20)
				{
					BEGIN = true;
					putText(framesOriginal, "START GESTURES", Point(30, 30), 1, 3, Scalar(255, 43, 0), 2); //blue
					
				}

				if (BEGIN == true)
				{
					putText(framesOriginal, " CAN DO GESTURES NOW ", Point(30, 60), 1, 2, Scalar(9, 255, 0), 2); //green
					//string invalue1;
					//stringstream ss_in1;
					//ss_in1 << wavein;
					//invalue1 = ss_in1.str();
					//putText(framesOriginal, "The in value before WaveIn is " + invalue1, Point(20, 300), 1, 1, Scalar(9, 255, 0), 2); //green

					/*wavein = Waving.WaveIn(red, green, framesThreshRED, framesThreshGREEN, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);

					string invalue;
					stringstream ss_in;
					ss_in << wavein;
					invalue = ss_in.str();
					putText(framesOriginal, "The in value after WaveIn is " + invalue, Point(20, 250), 1, 1, Scalar(9, 255, 0), 2); //green


					if (wavein == true)
					{
						staytrue = true;
					}

					string invalue1;
					stringstream ss_in1;
					ss_in1 << staytrue;
					invalue1 = ss_in1.str();
					putText(framesOriginal, "The in value after WaveOut is " + invalue1, Point(20, 300), 1, 1, Scalar(9, 255, 0), 2); //green*/

					if (staytrue == true)
					{
						waveout = Waving.WaveOut(red, green, framesThreshRED, framesThreshGREEN, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);
					}
				}
				else
				{
					putText(framesOriginal, " CAN'T DO GESTURES YET ", Point(30, 60), 1, 2, Scalar(0, 0, 255), 2); //red
				}
				//then find the blues
				//cvtColor(framesOriginal, framesHSV, COLOR_BGR2HSV);
				//inRange(framesHSV, blue.getminHSV(), blue.getmaxHSV(), framesThresh);
				//remove.noise(framesThresh);
				//trackColor.track_Color(blue, framesThresh, framesHSV, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);
				//get_Coords.X_Coord(red, blue, framesThresh, framesHSV, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);


				//then the greens
				//cvtColor(framesOriginal, framesHSV, COLOR_BGR2HSV);
				//inRange(framesHSV, green.getminHSV(), green.getmaxHSV(), framesThresh);
				//remove.noise(framesThresh);
				//trackColor.track_Color(green, framesThresh, framesHSV, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);


				//then the yellows
				//cvtColor(framesOriginal, framesHSV, COLOR_BGR2HSV);
				//inRange(framesHSV, yellow.getminHSV(), yellow.getmaxHSV(), framesThresh);
				//remove.noise(framesThresh);
				//trackColor.track_Color(yellow, framesThresh, framesHSV, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);


				//then the oranges
				//cvtColor(framesOriginal, framesHSV, COLOR_BGR2HSV);
				//inRange(framesHSV, orange.getminHSV(), orange.getmaxHSV(), framesThresh);
				//remove.noise(framesThresh);
				//trackColor.track_Color(orange, framesThresh, framesHSV, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);

				//get_Coords.X_Coord(red, blue, framesThresh, framesHSV, framesOriginal, windowNameGaussThresh, objectArea_min, objectArea_max);


			}

			// pop up the windows
			imshow(windowNameOriginal, framesOriginal);
			waitKey(30);
		}
	
return 0;
}