#include "classes.h"

Individual::Individual() {
	dead = false;
	dispersed = false;
}

std::default_random_engine p_generator; //for poisson dist
std::default_random_engine u_generator; //for uniform dist

void Individual::reproduction() { //population sizes and dispersal probabilities

	float ExpOff = Lambda / (1 + abs(Lambda - 1)*pow((current_patch_num->density / current_patch_num->K), b)); //expected offspring in current patch

	poisson_distribution<int> pois_dist(ExpOff);
	int NOff = pois_dist(p_generator); //how many offspring does it have

	if (NOff > 0) { //if it produced offspring,
		for (int joff = 0; joff < NOff; joff++) { //each juvenile

			current_patch_num->density++; //increase current patch density
			Individual* pIndiv = new Individual(); //create a new individual instance
			pIndiv->disp_prob = disp_prob; //let it inherit its parent's dispersal information
			pIndiv->current_patch_num = current_patch_num; //set patch information

			//set dispersal probabilities
			uniform_real_distribution<float> unif_dist1(0.0, 1.0);
			uniform_real_distribution<float> unif_dist2(-0.02, 0.02);

			float ran_mrate = unif_dist1(u_generator); //random mutation rate
			if (ran_mrate < mrate) { //if a mutation occurs
				float ran_disp = unif_dist2(u_generator); //random dispersal probability
				float new_disp = pIndiv->disp_prob + ran_disp;

				if (new_disp < 0) { //if the change has made it negative, just make it 0
					new_disp = 0;
				}
				else if (new_disp > 1) {
					new_disp = 1;
				}
				pIndiv->disp_prob=new_disp; //change the dispersal prob of the offspring
			}
			current_patch_num->indiv_vec.push_back(pIndiv); //add it to the patch individual vector
		}
	}
	dead = true; //individuals die after reproduction, even if they haven't produced any offspring. there are no overlapping generations
	current_patch_num->density--; //take away one from density
}


bool Individual::dispersal(Landscape& ls1) {
	int patch_dest = current_patch_num->patch_ID;

	uniform_real_distribution<float> unif_dist1(0.0, 1.0);
	float ran_disp = unif_dist1(u_generator);
	float ran_curr = unif_dist1(u_generator);
	float ran_cost = unif_dist1(u_generator);

	if (disp_prob > ran_disp) { //if the individual disperses
		//movement between patches
		if (ls1.current > ran_curr) { patch_dest = current_patch_num->patch_ID - 1; } //upstream. ie if current=0.1, then only 10% will go upstream, the rest will go downstream
														//this is just the converse of saying "a current is .9 strength=90% will go downstream". the current here is represented as "probability of going against the current"
														//but really, ran_current is the prob that this particular individual goes downstream.
		else { patch_dest = current_patch_num->patch_ID + 1; } //downstream

		//for reflective boundaries
		if (patch_dest < 0) { patch_dest = 0; }
		if (patch_dest > ls1.npatches - 1) { patch_dest = ls1.npatches - 1; } //it has to be -1 because C++ counts from 0 and i cant do dsize[40], it is out of bounds

		//cost of dispersal
		if (ran_cost < dispcost) { patch_dest = -9; dead = true; } //ie if dispcost=.1 that means 10% die, it's basically a mortality measure. so if ran_cost<dispcost then the individual dies
													  //if this happens, what happens to densities? decrease by 1? otherwise current patch numbers will always reflect this indiv!
		if (patch_dest != -9) {
			ls1.patch_vec[patch_dest]->density++; //increase destination patch density by 1
			current_patch_num->density--; //decrease current patch density by 1
			dispersed = true;
			current_patch_num = ls1.patch_vec[patch_dest]; //update current patch information
			return true;
		}
	}
	else {
		return false;
	}
}
