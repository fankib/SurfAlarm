#include "NeuronalNet.h"



NeuronalNet::NeuronalNet(){
	CreateNet();
}


NeuronalNet::~NeuronalNet(){
}

void InitRandom(vector<double> *vec, int count) {
	for (int i = 0; i < count; i++) {
		vec->push_back(RandomClamped());
	}
}

void NeuronalNet::CreateNet() {
	// init random weights:

	
	InitRandom(&inputWeights, 71);
	InitRandom(&hiddenBias, 25);
	InitRandom(&hiddenWeights, 25);
	InitRandom(&outputBias, 1);
	
}

void NeuronalNet::PutWeights(const vector<double> &weights) {
	for (int i = 0; i < 71; i++) {
		inputWeights[i] = weights[i];
	}
	for (int i = 0; i < 25; i++) {
		hiddenBias[i] = weights[71+i];
	}
	for (int i = 0; i < 25; i++) {
		hiddenWeights[i] = weights[71+25+i];
	}
	for (int i = 0; i < 1; i++) {
		outputBias[i] = weights[71+25+25+i];
	}
}

double NeuronalNet::Train(const Sample &sample, ResponseState expectedState) {
	
	return 1.0;
}

vector<double> NeuronalNet::Evaluate(const Sample &sample) {

	vector<double> inputs; // 95 Inputs // 71 inputs
	for (int i = 0; i < Parameter::subImageWidth; i++) {
		inputs.push_back(sample.gusts[i]);
		inputs.push_back(sample.windSpeeds[i]);
		// ignoriere direction:
		// inputs.push_back(sample.directions[i]);
	}
	for (int i = 0; i < Parameter::subImageWidth - 1; i++) {
		inputs.push_back(sample.changeDirections[i]);
	}

	vector<double> hiddenInputs; // 25 inputs

	// für input 1-48, immer zwei "zusammenfassen"
	double netInput = 0;
	for (int i = 0; i < 48; i++) {
		netInput += inputs[i] * inputWeights[i];
		if (i % 2 == 1) {
			// add bias
			// netInput += hiddenBias[(i - 2) / 3] * (-1);
			netInput += hiddenBias[(i - 1) / 2] * (-1);
			netInput = Sigmoid(netInput, Parameter::sigmoidResponse);
			hiddenInputs.push_back(netInput);
			netInput = 0;
		}
	}
	// für input 48-71 (23 change directions)
	netInput = 0;
	for (int i = 48; i < 71; i++) {
		netInput += inputs[i] * inputWeights[i];
	}
	netInput += hiddenBias[24] * (-1);
	netInput = Sigmoid(netInput, Parameter::sigmoidResponse);
	hiddenInputs.push_back(netInput);

	// Work With Hidden Layer:
	vector<double> output; // 1 outputs

	for (int i = 0; i < 1; i++) {
		netInput = 0;
		for (int j = 0; j < 25; j++) {
			netInput += hiddenInputs[i * 25 + j] * hiddenWeights[i * 25 + j];
		}
		netInput += outputBias[i] * (-1);

		// special sigmoid:
		
		double deltaX;
		double deltaY;
		if (netInput < 0) {
			// shift left:
			deltaX = 2.297;
			deltaY = 0;
		} else {
			deltaX = -2.297;
			deltaY = 0.5;
		}
		double x = (netInput * 2.0) + deltaX;
		netInput = Sigmoid(x, Parameter::sigmoidResponse);
		netInput = netInput * 0.5 + deltaY;
		// */
		/*
		more or less the same:

		double deltaX = 2.297;
		double x = (netInput * 2.0);
		netInput = 0.5 * Sigmoid(x + deltaX, Parameter::sigmoidResponse) + 0.5 * Sigmoid(x - deltaX, Parameter::sigmoidResponse);
		// */

		output.push_back(netInput);
	}

	return output;

	/*
	double no = output[0];
	double maybe = output[1];
	double yes = output[2];

	// cout << "No:\t" << no << endl;
	// cout << "Maybe:\t" << maybe << endl;
	// cout << "Yes:\t" << yes << endl;

	ResponseState result = no > maybe ? ResponseState::No : ResponseState::Maybe;
	if (yes > no || yes > maybe) {
		result = ResponseState::Yes;
	}
	return result; */
}

//-------------------------------Sigmoid function-------------------------
//
//------------------------------------------------------------------------
double NeuronalNet::Sigmoid(double netinput, double response)
{
	return (1 / (1 + exp(-netinput / response)));
}