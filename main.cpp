#include "classes.h"


int main() {
	int reps;
	cout << "how many reps are needed?" << endl;
	cin >> reps;

	//initialise landscape and population
	Landscape ls1;
	Population p1;

	//need to initialise individuals! either in each patch separately or in the population, then assign StartInit amount to each patch

	float* results = new float[ls1.npatches*reps];
	for (int i = 0; i < ls1.npatches; i++) {
		for (int j = 0; j < reps; j++) {
			results[i*reps + j] = 0;
		}
	}
	ofstream fout;

	for (int ci = 0; ci < 5; ci++) {
		ls1.set_current(0.0 + ci * 0.1);
		cout << "current: " << ls1.current << endl;

		for (int r = 0; r < reps; r++) {
			cout << "rep: " << r << endl;


			for (int gen = 0; gen < 1000; gen++) { //let the model run for 1000 generations
				//first all the patches reproduce
				for (int p = 0; p < ls1.npatches; p++) {//for each patch
					int density = ls1.patch_vec[p]->indiv_vec.size(); //this is to ensure reproduction only acts on current generation
					for (int i = 0; i < density; i++) {//loop through individuals in the vector
						if (!ls1.patch_vec[p]->indiv_vec[i]->dead) { //since there are no overlapping generations
																	//need to make sure it hasnt reproduced yet
							ls1.patch_vec[p]->indiv_vec[i]->reproduction();
						}
					}
				}

				//at the end of this step, all individuals in the patch vector should either be dead or new juveniles

				cout << "all patches have reproduced" << endl;

				//reproduction and dispersal need to be separate so that dispersal can act on the new juveniles

				//then all the patches disperse
				for (int p = 0; p < ls1.npatches; p++) {
					int density = ls1.patch_vec[p]->indiv_vec.size();
					for (int i = 0; i < density; i++) {
						if (!ls1.patch_vec[p]->indiv_vec[i]->dead && !ls1.patch_vec[p]->indiv_vec[i]->dispersed) { //if they haven't dispersed yet (this is to avoid acting on newly added juveniles that dispersed from another patch
							bool disp;
							disp = ls1.patch_vec[p]->indiv_vec[i]->dispersal(ls1);
							if (disp) {
								ls1.patch_vec[p]->add_indiv(ls1.patch_vec[p]->indiv_vec[i]); //add indiv pointer to new patch vector
							}
						}
					}
				}
				//by the end of this step, patch indiv vector will have indivs that are have repro=dead, have emigrated=dispersed
				cout << "all patches have dispersed" << endl;

			} //end of gen loop



			//need to clear out individuals from previous rep in each patch
			for (int p = 0; p < ls1.npatches; p++) {
				int pdensity = ls1.patch_vec[p]->indiv_vec.size();
				for (int i = 0; i < pdensity; i++) {
					delete ls1.patch_vec[p]->indiv_vec[i];
				}
				delete ls1.patch_vec[p];
			}
			cout << "deleted individuals and patches" << endl;


			for (int p = 0; p < ls1.npatches; p++) {
				cout << "patch " << p << ":";
				float patch_rep_sum = 0;
				for (int i = 0; i < ls1.patch_vec[p]->indiv_vec.size(); i++) {
					if (!ls1.patch_vec[p]->indiv_vec[i]->dead) {
						patch_rep_sum = patch_rep_sum + ls1.patch_vec[p]->indiv_vec[i]->disp_prob;
					}
				}
				results[p*reps + r] = results[p*reps + r] / ls1.patch_vec[p]->density;//this takes the mean disp probability for each patch for each rep
				cout << results[p*reps + r] << endl;
				//cout << "taken average ";
				//cout << results[0] << endl;
			}
		} //end of rep loop

		stringstream ss;
		ss << "../Results/mean_disp_prob_c" << ls1.current << ".txt";
		fout.open(ss.str().c_str());

		for (int line = 0; line < ls1.npatches; line++) {
			for (int col = 0; col < reps; col++) {
				fout << results[line*reps + col] << " ";
			}
			fout << endl;
		}

		fout.close();

		delete[] results;

	}//end of current loop

	cin.get();
	return 0;
}