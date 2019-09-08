#pragma once
#include "species.h"
/* forward declaration so each class can know 	/
/ about the other's existance ahead of time	   */
class species;
class individual;

class environmentBox {
public:
	int food,
		occupancy;
	individual *occupant1, *occupant2;

	environmentBox() {
		food = 0;
		occupancy = 0;
		occupant1 = nullptr;
		occupant2 = nullptr;
	}

	void insertFood(int num = 10) {
		food += num;
	}
	int getFood() {
		return food;
	}
	void eatFood(int ate) {
		food -= ate;
	}

	int getOccupancy() {
		return occupancy;
	}
	void insertOccupant(individual *occupant_in) {
		if (occupant1 == nullptr) occupant1 = occupant_in;
		else occupant2 = occupant_in;
		occupancy++;
	}
	void removeOccupant(individual *occupant_in) {
		if (occupant1 == occupant_in) occupant1 = nullptr;
		else occupant2 = nullptr;
		--occupancy;
	}

	individual* getOccupant1() {
		return occupant1;
	}
	individual* getOccupant2() {
		return occupant2;
	}

};

void wipe(environmentBox envPtr[], int size) {
	for (int i = 0; i < size; i++) {
		(envPtr + i)->food = 0;
	}
}
