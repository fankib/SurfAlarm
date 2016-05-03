#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Controller.h"


// forward definition:
void doStaticJob();

int main(int argc, const char* argv[]) {

	srand(time(NULL));

	doStaticJob();

	return 0;
}

void doStaticJob() {


	Parameter::LoadIniParameter();
	Controller controller;

	// setup 

	system("curl -o temp_strandbad_biel.png http://strandbad-biel.meteobase.ch/chart_wind.php?lang=de");
	system("curl -o temp_wsa_ipsach.png http://wsa-ipsach.meteobase.ch/chart_wind.php?lang=de");

	// string strImage = Parameter::pathToOriginals + "original6.png";
	image_rgb image_strandbad("temp_strandbad_biel.png", png::require_color_space<png::rgb_pixel>());
	image_rgb image_wsaipsach("temp_wsa_ipsach.png", png::require_color_space<png::rgb_pixel>());

//	string strSample = Parameter::pathToTrainingSet + Parameter::pathToMaybe + "original.png.sample.0.png";
//	image_rgb *imSample = new image_rgb(strSample, png::require_color_space<png::rgb_pixel>());
//	Sample sample(imSample);
	// ----
	

	// create samples:
	// controller.imageManager.CreateTrainingSamplesFromOriginals();

	// train the net:
	// controller.Train();

	// train with genalg:
	// controller.TrainGenAlg();

	// check:
	cout << "Strandbad: ";
	controller.Check(image_strandbad);

	cout << "WSA-Ipsach: ";
	controller.Check(image_wsaipsach);

	/* 
	vector_sample* samples = controller.imageManager.GetYesSamples();
	
	for (int i = 0; i < samples->size(); i++) {
		controller.CheckSample(*(samples->at(i)));
	}
	*/
}
