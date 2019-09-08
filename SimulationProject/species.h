#pragma once
#include <string>
#include <list>
#include "environment.h"
using namespace std;

// Generate random number without preference
int rangeRandomAlg2(int min, int max) {
	int n = max - min + 1;
	int remainder = RAND_MAX % n;
	int x;
	do {
		x = rand();
	} while (x >= RAND_MAX - remainder);
	return min + x % n;
}


class species {
public:
	string name;
	int id,
		food_needed_for_survive,
		food_needed_for_reproduce,
		food_found,
		produce,
		died,
		population;
	bool meat_eater;
	char y_or_n;
	int box_number_landed_on;
	// if( 0 < food_found < 10) chance of survive = (food_found * 10) %
	// if( 10 < food_found < 20) chance of reproduce = ((food_found - 10) * 10) %
	int speed,
		sense_to_food,
		sense_to_danger;
	// Types: worm/lizard(0) mice/pigeon(1) cat/owl(2) hawk(3) pitbul(4) hyiena(5) deer(6) tiger(7) bloody python(8) bear(9) 
	//      : meat_eater(true or false)
	int size, size_it_can_eat_min, size_it_can_eat_max;

	//***** FUNCTIONS *****//
	species(int survive1 = 10, int reproduce1 = 20, int population1 = 5) {
		food_found = 0;
		food_needed_for_survive = survive1;
		food_needed_for_reproduce = reproduce1;
		population = population1;
		produce = 0;
		died = 0;
		meat_eater = false;
	}

	void eatSpecie() {

	}

	int getPop() {
		return population;
	}

	void species_create() {
		cout << "\nEnter specie's name: ";
		cin >> name;
		cout << "Enter amount of food needed to survive: ";
		cin >> food_needed_for_survive;
		cout << "Enter amount of food needed to reproduce: ";
		cin >> food_needed_for_reproduce;
		cout << "Enter starting poplulation for species: ";
		cin >> population;
		produce = population;
		cout << "Enter specie size: ";
		cin >> size;
		cout << "Is " << name << " a meat eater? (Y / N)";
		cin >> y_or_n;
		if (toupper(y_or_n) == 'Y') meat_eater = true;
		if (meat_eater) {
			cout << "What is min size " << name << " can eat?";
			cin >> size_it_can_eat_min;
			cout << "What is max size " << name << " can eat?";
			cin >> size_it_can_eat_max;
		}

	}

	void species_display() {
		cout << "\nName:                         " << name
			<< "\nFood Found:                   " << food_found
			<< "\nFood needed for survival:     " << food_needed_for_survive
			<< "\nFood needed for reproduce:    " << food_needed_for_reproduce
			<< "\nPopulation:                   " << population << endl;
	}

	/*	void StepOutBox(environmentBox envPtr[]) {
	if ((envPtr + box_number_landed_on)->getFood() == 0) return;
	if (food_found > food_needed_for_reproduce) return;
	else if ((envPtr + box_number_landed_on)->getOccupancy() == 2 && (envPtr + box_number_landed_on)->getFood() > 0) {
	food_found += 10;
	(envPtr + box_number_landed_on)->eatFood(10);
	}
	else if ((envPtr + box_number_landed_on)->getFood() > 0) {
	food_found += 20;
	(envPtr + box_number_landed_on)->eatFood(20);
	}
	}							  */

	int EndOfDay() {
		population += (produce - died);
		died = 0;
		produce = 0;
		cout << name << string(10 - name.length(), ' ') << ": " << string(population, '=') << "\n";
		return food_found;
	}

	int GetReproducing() {
		return produce;
	}
	int GetDied() {
		return died;
	}

	void NewDay() {
		produce = 0;
		food_found = 0;
	}

	bool can_eat(species *s_ptr) {
		if (size_it_can_eat_min <= s_ptr->size && s_ptr->size <= size_it_can_eat_max) return true;
		else return false;
	}

	void print_stats() {
		cout << name << string(10 - name.length(), ' ') << ": " << string(population / 10, '=') << "\n";
		//		cout << name << string(10 - name.length(), ' ') << ": " << population << "\n";
	}
};

class individual {
public:
	int id,
		food_ate;
	species *Ptr_to_specie;

	individual(int id1 = 0, int food_ate1 = 0, species *s_ptr = nullptr) {
		id = id1;
		food_ate = food_ate1;
		Ptr_to_specie = s_ptr;
	}

	//** Place Individuals In Environment **//
	void StepInBox(environmentBox envPtr[], int size, individual *indiv_ptr1) {
		if (food_ate >= Ptr_to_specie->food_needed_for_reproduce) return;
		int box_number_landed_on;
		do {
			box_number_landed_on = rangeRandomAlg2(0, size);
		} while ((envPtr + box_number_landed_on)->getOccupancy() >= 2);
		(envPtr + box_number_landed_on)->insertOccupant(indiv_ptr1);
	}
};