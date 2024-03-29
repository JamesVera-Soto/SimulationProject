#include "stdafx.h"
#include <iostream>
#include <vector>
#include "species.h"
#include <Windows.h>
#include <time.h>		 // Needed to generate random numbers

// Prototypes
void ScanEnv(environmentBox envPtr[], int size);
void clear_off_occupants(environmentBox env_ptr[], int size);
void menu_options();

int main()
{
	list<individual> indiv;
	list<individual>::iterator indiv_it;
	individual *indiv_ptr;

	// For text color change: SetConsoleTextAttribute(screen, color); color#s 0-16	//
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	// initialize random seed //
	srand(time(NULL));
	// generate random number between 0 and 99 //
	int food_to_be_in_env;

	const int ROWS = 50, COLS = 50;
	environmentBox env[ROWS][COLS];
	vector<species> VectorOfSpecies;

	cout << "Enter amount of food wanted: ";
	cin >> food_to_be_in_env;
	while (food_to_be_in_env > ROWS *COLS) {
		cout << "Food over capacity!\nEnter new amount: ";
		cin >> food_to_be_in_env;
	}
	//** Create Environment **//
	for (int i = 0, rr = rand() % ROWS, rc = rand() % COLS; i < food_to_be_in_env && i < ROWS * COLS; rr = rand() % ROWS, rc = rand() % COLS, i++) {
		if (env[rr][rc].getFood() == 0) env[rr][rc].insertFood(20);
		else --i;
	}
	// Set text color to white //
	SetConsoleTextAttribute(screen, 15);
	//** Display environment **//
	for (int i = 0, num = 1; i < ROWS; num++, i++) {
		if (num < 10) cout << " ";
		cout << num << ": ";
		for (int j = 0; j < COLS; j++) {
			if ((env[i][j].getFood()) > 0) {
				SetConsoleTextAttribute(screen, 10);
				cout << (char)254 << " ";
				SetConsoleTextAttribute(screen, 15);
			}
			else cout << (char)254 << " ";
		}
		cout << "\n";
	}
	//** Create Species **//
	cout << "How many species do you want to create? ";
	int amount;
	cin >> amount;
	for (int i = 0; i < amount; i++) {
		VectorOfSpecies.push_back(species());
		VectorOfSpecies[i].species_create();
	}


	//******************************************************** d Days ********************************************************************************//
	for (int d = 0; d < 365; d++) {											  // 365 days
																			  //** 1 Day each iteration **//
		for (int s = 0; s < 4; s++) {										  // 5 steps in a day
																			  // step in env box
			for (int i = 0; i < VectorOfSpecies.size(); i++) {				  // i = species
				if (VectorOfSpecies[i].population == 0) continue;
				for (; VectorOfSpecies[i].produce > 0; VectorOfSpecies[i].produce--) {// p starting at Specie's Population going to 0
					indiv.push_back(individual(1, 0, &VectorOfSpecies[i]));	// creates individual
				}
				for (indiv_it = indiv.begin(); indiv_it != indiv.end(); indiv_it++) {
					indiv_it->StepInBox(env[0], ROWS * COLS - 1, &(*indiv_it)); // Place individual into environment
				}
				if (s == 0) VectorOfSpecies[i].NewDay();			   // produce = 0
			}

			ScanEnv(&env[0][0], ROWS * COLS);								// Scans environment after each step 
			clear_off_occupants(&env[0][0], ROWS * COLS);					// Reset occupancy and occupant pointers
		}

		// end of day
		indiv_it = indiv.begin();
		while (indiv_it != indiv.end()) {
			if (indiv_it->food_ate < indiv_it->Ptr_to_specie->food_needed_for_survive) indiv_it->id = -1;	  // if starved	id = -1
			if (indiv_it->id == -1) {						// -1 id means that individual got eaten or starved
				indiv_it->Ptr_to_specie->population--;			//** decrement population
				indiv.erase(indiv_it++);						//** erase dead individual from list
			}
			else if (indiv_it->food_ate >= indiv_it->Ptr_to_specie->food_needed_for_reproduce) {   // ate enough to reproduce
				indiv_it->Ptr_to_specie->population++;													//** increment population
				indiv_it->Ptr_to_specie->produce++;														//** increment produce
				indiv_it->food_ate = 0;																	//** reseto food_ate to 0
				indiv_it++;
			}
			else {
				indiv_it->food_ate = 0;
				indiv_it++;
			}
		}
		menu_options();


		//** Clear Individuals' Food_Ate form previous day **//
		for (indiv_it = indiv.begin(); indiv_it != indiv.end(); indiv_it++) {
			indiv_it->food_ate = 0;
		}

		cout << string(5, '\n');  // makes space or graph
								  // Print Stats //
		for (int i = 0; i < VectorOfSpecies.size(); i++) {
			VectorOfSpecies[i].print_stats();
		}

		//** Wipe Environment of Food **//
		wipe(&env[0][0], ROWS * COLS);
		//** Create Environment **//
		for (int i = 0, rr = rand() % ROWS, rc = rand() % COLS; i < food_to_be_in_env && i < ROWS * COLS; rr = rand() % ROWS, rc = rand() % COLS, i++) {
			if (env[rr][rc].getFood() == 0) env[rr][rc].insertFood(20);
			else --i;
		}


		//		Sleep(500);

	} //*****************************************************************************************************************************************************//

	  // Display species
	for (int i = 0; i < amount; i++) {
		VectorOfSpecies[i].species_display();
	}
	//** Display environment **//
	for (int i = 0, num = 1; i < ROWS; num++, i++) {
		if (num < 10) cout << " ";
		cout << num << ": ";
		for (int j = 0; j < COLS; j++) {
			if ((env[i][j].getFood()) > 0) {
				SetConsoleTextAttribute(screen, 10);
				cout << (char)254 << " ";
				SetConsoleTextAttribute(screen, 15);
			}
			else cout << (char)254 << " ";
		}
		cout << "\n";
	}
	menu_options();
	return 0;
}


//** Scan Environment after step **//
void ScanEnv(environmentBox envPtr[], int size) {
	for (int i = 0; i < size; i++) {
		if ((envPtr + i)->occupancy == 0) continue;
		if ((envPtr + i)->occupancy == 1) {
			if ((envPtr + i)->getFood() == 0 || ((((envPtr + i)->occupant1)->Ptr_to_specie)->meat_eater)) continue;
			else { // food > 0 && only occupant eat plant stuff
				(envPtr + i)->occupant1->food_ate += (envPtr + i)->food;
				(envPtr + i)->occupant1->Ptr_to_specie->food_found += (envPtr + i)->food;
				(envPtr + i)->food = 0;
			}
		}
		else if ((envPtr + i)->occupancy == 2) {
			if ((envPtr + i)->occupant1->Ptr_to_specie->meat_eater || ((envPtr + i)->occupant2->Ptr_to_specie->meat_eater)) {	// 1 or 2 meat eaters
				if ((envPtr + i)->occupant1->Ptr_to_specie->can_eat((envPtr + i)->occupant2->Ptr_to_specie)
					&& (envPtr + i)->occupant1->food_ate < (envPtr + i)->occupant1->Ptr_to_specie->food_needed_for_reproduce) {
					(envPtr + i)->occupant1->food_ate += (10 * (envPtr + i)->occupant2->Ptr_to_specie->size);
					(envPtr + i)->occupant2->id = -1;
					(envPtr + i)->occupant2 = nullptr;
					(envPtr + i)->occupancy -= 1;
				}
				else if ((envPtr + i)->occupant2->Ptr_to_specie->can_eat((envPtr + i)->occupant1->Ptr_to_specie)
					&& (envPtr + i)->occupant2->food_ate < (envPtr + i)->occupant2->Ptr_to_specie->food_needed_for_reproduce) {
					(envPtr + i)->occupant2->food_ate += (10 * (envPtr + i)->occupant1->Ptr_to_specie->size);
					(envPtr + i)->occupant1->id = -1;
					(envPtr + i)->occupant1 = nullptr;
					(envPtr + 1)->occupancy -= 1;
				}
			}
			else if (!((envPtr + i)->occupant1->Ptr_to_specie->meat_eater) && !((envPtr + i)->occupant2->Ptr_to_specie->meat_eater)) {
				if ((envPtr + i)->food == 0) continue;
				else { // Both eat plant stuff
					if (((envPtr + i)->occupant1->food_ate < (envPtr + i)->occupant1->Ptr_to_specie->food_needed_for_reproduce)
						&& ((envPtr + i)->occupant2->food_ate < (envPtr + i)->occupant2->Ptr_to_specie->food_needed_for_reproduce)) {
						(envPtr + i)->occupant1->food_ate += ((envPtr + 1)->food / 2);
						(envPtr + i)->occupant1->Ptr_to_specie->food_found += 10;
						(envPtr + i)->occupant2->food_ate += ((envPtr + 1)->food / 2);
						(envPtr + i)->occupant2->Ptr_to_specie->food_found += 10;
						(envPtr + i)->food = 0;
					}
				}
			}
		}
	}
}


/* end of day
for (int i = 0; i < VectorOfSpecies.size(); i++) {
replenish = VectorOfSpecies[i].EndOfDay();
/ Replenish Environment /
for (int i = 0, rr = rand() % ROWS, rc = rand() % COLS; i < replenish; rr = rand() % ROWS, rc = rand() % COLS, i++) {
if (env[rr][rc].getFood() == 0) env[rr][rc].insertFood();
else --i;
}
}  */

void clear_off_occupants(environmentBox env_ptr[], int size) {
	for (int i = 0; i < size; i++) {
		(env_ptr + i)->occupancy = 0;
		(env_ptr + i)->occupant1 = nullptr;
		(env_ptr + i)->occupant2 = nullptr;
	}

}

void menu_options() {
	cout << "Press ENTER to advance to next day" << endl;	// menu options in development
	char ch = getchar();
}