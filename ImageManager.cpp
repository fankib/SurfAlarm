#include "ImageManager.h"

ImageManager::~ImageManager() {
	if (samplesNo != nullptr) {
		delete samplesNo;
	}
	if (samplesNo != nullptr) {
		delete samplesMaybe;
	}
	if (samplesNo != nullptr) {
		delete samplesYes;
	}
}

void CutImageSample(image_rgb* sample, const image_rgb &original, const int offsetx = 0) {
	for (png::uint_32 w = 0; w < sample->get_width(); ++w) {
		for (png::uint_32 h = 0; h < sample->get_height(); ++h) {
			sample->set_pixel(w, h, original.get_pixel(Parameter::subImageX - offsetx + w, Parameter::subImageY + h));
		}
	}
}

// -------------------------------
// Generate TrainingSamples
// -------------------------------
void ImageManager::ProcessOriginal(const string &name) {
	string path = Parameter::pathToOriginals + name;
	cout << "Process" << path << endl;

	image_rgb original(path, png::require_color_space<png::rgb_pixel>());

	// create 23 samples:
	for (int i = 0; i < 23; i++) {
		int offsetx = i * Parameter::subImageWidth;
		// cut image:
		image_rgb sample(Parameter::subImageWidth, Parameter::subImageHeight);
		CutImageSample(&sample, original, offsetx);
		// write:
		string sampleName = name + ".sample." + to_string(i) + ".png";
		cout << "write: " << sampleName << endl;
		sample.write(Parameter::pathToSampledOriginals + sampleName);		
	}

};

void ImageManager::CreateTrainingSamplesFromOriginals() {
	DIR* dir;
	struct dirent* ent;

	dir = opendir(Parameter::pathToOriginals.c_str());
	if (dir == NULL) {
		cerr << "PathToOriginals not found!" << endl;
		assert(0);
	}

	while ((ent = readdir(dir)) != NULL) {
		string name = string(ent->d_name);
		if (has_suffix(name, ".png")) {
			ProcessOriginal(name);
		}
	}

	closedir(dir);
};
// -------------------------------

// -------------------------------
// Create Sample from Original
// -------------------------------
Sample* ImageManager::CreateSample(const image_rgb &image) {
	image_rgb *imSample = new image_rgb (Parameter::subImageWidth, Parameter::subImageHeight);
	CutImageSample(imSample, image);
	Sample *result = new Sample(imSample);
	return result;
}
// -------------------------------


// -------------------------------
// Read Training Samples
// -------------------------------

void ImageManager::ReadSamples(const string &directory, vector_sample* result) {
	DIR* dir;
	struct dirent* ent;

	string path = Parameter::pathToTrainingSet + directory;

	dir = opendir(path.c_str());
	if (dir == NULL) {
		cerr << "TrainingSet not found!" << endl;
		assert(0);
	}	

	Sample* sample = nullptr;
	image_rgb* image = nullptr;

	while ((ent = readdir(dir)) != NULL) {
		string name = string(ent->d_name);
		if (has_suffix(name, ".png")) {
			string file = path + name;
			image = new image_rgb(file);
			sample = new Sample(image, name);
			result->push_back(sample);
		}
	}

	closedir(dir);
}

vector_sample* ImageManager::GetNoSamples() {
	if (samplesNo != nullptr) {
		delete samplesNo;
	}
	samplesNo = new vector_sample();
	ReadSamples(Parameter::pathToNo, samplesNo);
	return samplesNo;
};

vector_sample* ImageManager::GetMaybeSamples() {
	if (samplesMaybe != nullptr) {
		delete samplesMaybe;
	}
	samplesMaybe = new vector_sample();
	ReadSamples(Parameter::pathToMaybe, samplesMaybe);
	return samplesMaybe;
};

vector_sample* ImageManager::GetYesSamples() {
	if (samplesYes != nullptr) {
		delete samplesYes;
	}
	samplesYes = new vector_sample();
	ReadSamples(Parameter::pathToYes, samplesYes);
	return samplesYes;
}
// -------------------------------