#pragma once
#include <vector>
#include <math.h>
#include <string>

#include <png++/png.hpp>

typedef png::image<png::rgb_pixel> image_rgb;

using namespace std;

class Sample{
private:
	image_rgb* image;
	string name;

	void ProcessImage();

public:

	vector<double> gusts;
	vector<double> windSpeeds;
	vector<double> directions;
	vector<double> changeDirections;

	// Sample(): image(nullptr){};
	Sample(image_rgb* image);
	Sample(image_rgb* image, string name);
	~Sample();
};

