#include "classes.h"

Landscape::Landscape() {

}

void Landscape::set_current(float new_current) {
	current = new_current;
}

void Landscape::set_npatches(int np) {
	npatches = np;
}

void Landscape::create_patch_vector() {
	for (int i = 0; i < npatches; i++) {
		Patch* pPatch = new Patch();
		patch_vec.push_back(pPatch);
		patch_vec[i]->patch_ID = i;
	}
}
