#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "ResponseState.h"
#include "Parameter.h"
#include "ImageManager.h"
#include "NeuronalNet.h"
#include "CGenAlg.h"

using namespace std;

class Controller {

private:

	NeuronalNet neuronalNet;
	CGenAlg* genAlg;

public:

	Controller();
	~Controller();

	ImageManager imageManager;

	// Save Net
	void SaveGenome(const SGenome &genome, const string &file);
	// Net
	void LoadGenome(SGenome &genome, const string &file);

	// Train the Net using classified Data
	void Train();
	void TrainGenAlg();
	// Check a given image
	double Check(const image_rgb &image);
	double CheckSample(const Sample &sample);
	// Check and let the user classify the image
	// ResponseState CheckAndVerify(/*image*/ const ResponseState &expectedResponse);

};