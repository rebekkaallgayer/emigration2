#include "classes.h"

Patch::Patch() {
	K = 50;
}


void Patch::add_indiv(Individual* pindiv) {
	indiv_vec.push_back(pindiv);
}
