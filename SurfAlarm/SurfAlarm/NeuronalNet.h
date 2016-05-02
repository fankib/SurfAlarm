#pragma once

#include <math.h>
#include <iostream>

#include "ResponseState.h"
#include "Sample.h"
#include "Parameter.h"
#include "utils.h"

class NeuronalNet {

private:

	vector<double> inputWeights; // 95 weights
	vector<double> hiddenBias; // 25 bias
	vector<double> hiddenWeights; // 75 weights
	vector<double> outputBias; // 1 bias

public:
	NeuronalNet();
	~NeuronalNet();

	// Create the net
	void CreateNet();

	double Train(const Sample &sample, ResponseState expected);

	// weights management:
	// vector<double>* GetWeights();
	void PutWeights(const vector<double> &weights);

	// inputs Sample to evaluate
	vector<double> Evaluate(const Sample &sample);

	//sigmoid response curve
	inline double Sigmoid(double activation, double response);
};

