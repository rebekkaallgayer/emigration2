#include "classes.h"

Population::Population() {
	b = 1;
}

void Population::pop_setup() {
	float InitDisp; float StartPop; int MaxInd;  float Lambda; float b; float current; float mrate; float dispcost;

	cout << "set the initial dispersal probability, starting population size, maximum individuals, lambda, carrying capacity, mutation rate, and dispersal cost" << endl;

	cin >> InitDisp >> StartPop >> Lambda >> K >> mrate >> dispcost;
	this->InitDisp = InitDisp;
	this->StartPop = StartPop;
	this->MaxInd = MaxInd;
	this->Lambda = Lambda;
	this->mrate = mrate;
	this->dispcost = dispcost;

	this->a = (pow(Lambda, 1 - b) - 1) / K;

}

