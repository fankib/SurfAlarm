#pragma once

#include <fstream>
// #include <windows.h>
#include <iostream>
#include <string>

using namespace std;

class Parameter{

public:
	
	static string pathToTrainingSet;
	static string pathToNo;
	static string pathToMaybe;
	static string pathToYes;
	static string pathToOriginals;
	static string pathToSampledOriginals;
	static string pathToGenoms;

	static int subImageX;
	static int subImageY;
	static int subImageWidth;
	static int subImageHeight;

	static double sigmoidResponse;

	static void LoadIniParameter();

};