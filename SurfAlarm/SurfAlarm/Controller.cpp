#include "Controller.h"


Controller::Controller() {
	genAlg = new CGenAlg(50, 0.6, 0.7, 122);
}

Controller::~Controller() {
	delete genAlg;
}

// ------------------
// Save & Load
// ------------------
union GenomEntity {
	char v1;
	double v2;
};

void Controller::SaveGenome(const SGenome &genome, const string &file) {
	ofstream out(file, ios::out | ios::binary);

	if (!out) {
		std::cerr << "Can not create genome file!" << std::endl;
		return;
	}

	for (int i = 0; i < genome.vecWeights.size(); i++) {
		GenomEntity data;
		data.v2 = genome.vecWeights[i];
		out.write(&data.v1, sizeof(data));
	}

	//std::copy(genome.vecWeights.begin(), genome.vecWeights.end(), std::ostream_iterator<char>(out));	
};

void Controller::LoadGenome(SGenome &genome, const string &file) {
	ifstream in(file, ios::in | ifstream::binary);
	if (!in) {
		std::cerr << "Can not read genome file!" << std::endl;
	}

	genome.vecWeights.clear();

	GenomEntity data;
	while (in.read(&data.v1, sizeof(data))) {
		genome.vecWeights.push_back(data.v2);
	}
};
// -------------------


// -------------------
// Training use Backpropagation
// -------------------
void Controller::Train() {
	vector_sample* samplesNo = imageManager.GetNoSamples();
	vector_sample* samplesMaybe = imageManager.GetMaybeSamples();
	vector_sample* samplesYes = imageManager.GetYesSamples();
	
	// train 10 times (maybe more)
	for (int j = 0; j < 10; j++) {
		double error = 0;

		// samples no:
		for (int i = 0; i < samplesNo->size(); i++) {
			error += neuronalNet.Train(*samplesNo->at(i), ResponseState::No);
		}

		// samples maybe:
		for (int i = 0; i < samplesMaybe->size(); i++) {
			error += neuronalNet.Train(*samplesMaybe->at(i), ResponseState::Maybe);
		}

		// samples yes:
		for (int i = 0; i < samplesYes->size(); i++) {
			error += neuronalNet.Train(*samplesYes->at(i), ResponseState::Yes);
		}
		
		cout << "Error iteration (" << j << "): " << error << endl;
	}

	cout << "Done!" << endl;
};

void calculateSuccess (int populationSize, vector<SGenome> &population, vector<NeuronalNet*>* nets, vector_sample* samplesNo, vector_sample* samplesMaybe, vector_sample* samplesYes) {
	vector<double> output;
	for (int j = 0; j < populationSize; j++) {
		double success = 0;
		double success_no = 0;
		double success_maybe = 0;
		double success_yes = 0;

		// samples no:
		for (int i = 0; i < samplesNo->size(); i++) {
			output = (*nets)[j]->Evaluate(*samplesNo->at(i));
			success_no += output[0] < 0.3 ? (1.0 - output[0]) : 0;
		}
		success_no /= samplesNo->size();

		// samples maybe:
		for (int i = 0; i < samplesMaybe->size(); i++) {
			output = (*nets)[j]->Evaluate(*samplesMaybe->at(i));
			success_maybe += 1.0 - abs(output[0] - 0.5);
		}
		success_maybe /= samplesMaybe->size();

		// samples yes:
		for (int i = 0; i < samplesYes->size(); i++) {
			output = (*nets)[j]->Evaluate(*samplesYes->at(i));
			success_yes += output[0] > 0.7 ? (output[0]) : 0;
		}
		success_yes /= samplesYes->size();

		success = success_no * success_maybe * success_yes;

		population[j].dFitness = success;
		cout << "Net " << j << ": " << success << " (success)" << endl;
	}

}

void splitSamples(vector_sample* samples, vector_sample* training, vector_sample* test) {
	for (int i = 0; i < samples->size(); i++) {
		if (i % 3 == 2) {
			test->push_back(samples->at(i));
		}
		else {
			training->push_back(samples->at(i));
		}
	}
}

// OR GenAlg:
void Controller::TrainGenAlg() {
	int populationSize = 50;
	
	// trainingdata
	vector_sample* samplesNo = imageManager.GetNoSamples();
	vector_sample* samplesMaybe = imageManager.GetMaybeSamples();
	vector_sample* samplesYes = imageManager.GetYesSamples();
	int maxCorrect = samplesNo->size() + samplesMaybe->size() + samplesMaybe->size();

	vector_sample* trainNo = new vector_sample();
	vector_sample* trainMaybe = new vector_sample();
	vector_sample* trainYes = new vector_sample();

	vector_sample* testNo = new vector_sample();
	vector_sample* testMaybe = new vector_sample();
	vector_sample* testYes = new vector_sample();

	// split samples into 2/3 for train and 1/3 test
	splitSamples(samplesNo, trainNo, testNo);
	splitSamples(samplesMaybe, trainMaybe, testMaybe);
	splitSamples(samplesYes, trainYes, testYes);

	// init population:
	vector<SGenome> population = genAlg->GetChromos();
	vector<NeuronalNet*>* nets = new vector<NeuronalNet*>();
	for (int i = 0; i < populationSize; i++) {
		NeuronalNet* pNeuro = new NeuronalNet();
		pNeuro->PutWeights(population[i].vecWeights);
		nets->push_back(pNeuro);
	}

	// Test load and save:
	// SaveGenome(population[0], Parameter::pathToGenoms + "genom0.gen");
	// LoadGenome(population[0], Parameter::pathToGenoms + "genom0.gen");

	int numGenerations = 250;
	int doGenerations = numGenerations;

	int generation = 0;
	while (doGenerations > 0) { 
		doGenerations--;
		generation++;
		cout << "Generation " << generation << ": " << endl;

		// train and calculate fitness
		calculateSuccess(populationSize, population, nets, trainNo, trainMaybe, trainYes);
		// ----
		
		cout << "epoche .." << endl << endl;

		// epoch and update:
		population = genAlg->Epoch(population);
		for (int i = 0; i < populationSize; i++) {
			(*nets)[i]->PutWeights(population[i].vecWeights);
		}

		// do test and evaluate:
		if (doGenerations == 0) {
			cout << endl << "Evaluate Progress:" << endl;
			calculateSuccess(populationSize, population, nets, testNo, testMaybe, testYes);
			double maxFitness = 0;
			SGenome maxGenom;
			for (int i = 0; i < populationSize; i++) {
				if (population[i].dFitness > maxFitness) {
					maxFitness = population[i].dFitness;
					maxGenom = population[i];
				}
			}

			cout << "Max fitness: " << maxFitness << endl;
			SaveGenome(maxGenom, Parameter::pathToGenoms + "maxGenom.gen");

			char buffer[100];
			cout << "Again (y/n): ";
			cin >> buffer;
			cout << endl;
			if (string(buffer).at(0) == 'y')	{
				doGenerations = numGenerations;
			}
		}

	}

	delete nets;
}

// -------------------


// -------------------
// Perform Checks
// -------------------
ResponseState toResponseState(double value) {
	if (value < 0.45) {
		return ResponseState::No;
	}
	if (value < 0.7) {
		return ResponseState::Maybe;
	}
	return ResponseState::Yes;
};


void printResponseState(ResponseState state) {
	std::string strOut = "No";
	if (state == ResponseState::Maybe) {
		strOut = "Maybe";
	}
	if (state == ResponseState::Yes) {
		strOut = "Yes";
	}
	std::cout << strOut;
}

double Controller::Check(const image_rgb &image) {

	Sample *sample = imageManager.CreateSample(image);

	double result = CheckSample(*sample);

	delete sample;
	return result;
};

double Controller::CheckSample(const Sample &sample) {
	SGenome genome;
	LoadGenome(genome, Parameter::pathToGenoms + "maxGenom.gen");
	neuronalNet.PutWeights(genome.vecWeights);

	double result = neuronalNet.Evaluate(sample)[0];
	cout << "Result (0..1): " << result << "\t";
	printResponseState(toResponseState(result));
	cout << endl;
	return result;
}

/* ResponseState Controller::CheckAndVerify(const ResponseState &expectedResponse ) {
	return ResponseState::No;
}; */
// -------------------