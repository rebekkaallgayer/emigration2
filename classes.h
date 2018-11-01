#pragma once

#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <fstream>  //stream class to both read and write from/to files
#include <string>
#include <math.h> //for atan2 and pow
#include <random> //for distributions
#include <sstream> //for adding strings together

using namespace std;

class Individual;


class Patch {
public:
	Patch();
	int patch_ID;
	int density;
	int K;

	vector<Individual*> indiv_vec;
	void add_indiv(Individual* pindiv);

};

class Landscape {
public:
	Landscape();

	int npatches;
	float current; //between 0 and 1 ( when 0 or 1 all the inds move same direction; when 0.5 equal in both directions)
	vector<Patch*> patch_vec;

	void set_current(float new_current);
	void set_npatches(int np);
	void create_patch_vector();
};



class Population {
public: 
	Population();

	int MaxInd;
	float InitDisp;
	int StartPop;
	float Lambda;
	int b;
	float a;
	float mrate; //mutation rate
	float dispcost; //cost of dispersal

	void pop_setup();

};

class Individual:public Population {
public:
	Individual();
	Patch* current_patch_num;
	float disp_prob;
	bool dead;
	bool dispersed;


	void reproduction();
	bool dispersal(Landscape& ls1);
};


#endif CLASSES_H