#include "road.h"
#include <iostream>

Road::Road(){};

void Road::print() {
    for (auto& cell : mRoad) {
        cell.print();
    }
    std::cout << std::endl;
}

void Road::applyUpdate() {
    // Copy the next road to the current road
    mRoad = mNextRoad;
    // Set the next road to be empty
    mNextRoad = std::vector<Cell>(mRoad.size());
}
