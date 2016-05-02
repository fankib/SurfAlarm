#pragma once

#include <cassert>
#include <iostream>
#include <vector>

#include "png++-0.2.9\png.hpp"
#include "dirent.h"

#include "Parameter.h"
#include "utils.h"
#include "Sample.h"

using namespace std;

typedef vector<Sample*> vector_sample;

// This class manages the pictures
class ImageManager {
private:

	vector_sample* samplesNo;
	vector_sample* samplesMaybe;
	vector_sample* samplesYes;

	void ProcessOriginal(const string &name);
	void ReadSamples(const string &directory, vector_sample* result);

public:

	ImageManager() :samplesNo(nullptr), samplesMaybe(nullptr), samplesYes(nullptr) {};
	~ImageManager();

	// use this to sample original images (used for supervised training)
	void CreateTrainingSamplesFromOriginals();

	// Cut out the Sample from an original
	Sample* CreateSample(const image_rgb &image);

	// get the No-Samples
	vector_sample* GetNoSamples();

	// get the Maybe-Samples
	vector_sample* GetMaybeSamples();

	// get the Yes-Samples
	vector_sample* GetYesSamples();

};