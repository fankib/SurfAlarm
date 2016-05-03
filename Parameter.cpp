#include "Parameter.h"

// --------------------------------------------
// initialize values:
string Parameter::pathToTrainingSet = "";
string Parameter::pathToNo = "";
string Parameter::pathToMaybe = "";
string Parameter::pathToYes = "";
string Parameter::pathToOriginals = "";
string Parameter::pathToSampledOriginals = "";
string Parameter::pathToGenoms = "";

int Parameter::subImageX = 0;
int Parameter::subImageY = 0;
int Parameter::subImageWidth = 0;
int Parameter::subImageHeight = 0;

double Parameter::sigmoidResponse = 1.0;
// --------------------------------------------

void Parameter::LoadIniParameter() {
	ifstream grab("params.ini");

	if (!grab) {
		std::cerr << "Can not find ini file!" << std::endl;
	}

	// TODO: We trust the userinput..
	char paramDescriptionBuffer[1024];
	char buffer[1024];

	grab >> paramDescriptionBuffer;
	grab >> buffer;
	Parameter::pathToTrainingSet = string(buffer);
	grab >> paramDescriptionBuffer;
	grab >> buffer;
	Parameter::pathToNo = string(buffer);
	grab >> paramDescriptionBuffer;
	grab >> buffer;
	Parameter::pathToMaybe = string(buffer);
	grab >> paramDescriptionBuffer;
	grab >> buffer;
	Parameter::pathToYes = string(buffer);
	grab >> paramDescriptionBuffer;
	grab >> buffer;
	Parameter::pathToOriginals = string(buffer);
	grab >> paramDescriptionBuffer;
	grab >> buffer;
	Parameter::pathToSampledOriginals = string(buffer);
	grab >> paramDescriptionBuffer;
	grab >> buffer;
	Parameter::pathToGenoms = string(buffer);
	
	grab >> paramDescriptionBuffer;
	grab >> Parameter::subImageX;
	grab >> paramDescriptionBuffer;
	grab >> Parameter::subImageY;
	grab >> paramDescriptionBuffer;
	grab >> Parameter::subImageWidth;
	grab >> paramDescriptionBuffer;
	grab >> Parameter::subImageHeight;

	grab >> paramDescriptionBuffer;
	grab >> Parameter::sigmoidResponse;
	
};




  
  
