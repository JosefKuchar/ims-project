#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include "simulation.h"

// const float p1 = 0.3;
// const float p2 = 0.3;
// const float p3 = 0.4;
// const float pb = 0.3;
// const int epochs = 36000;

int main() {
    Simulation sim;
    sim.run(8);
    return 0;
}
