#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include "args.h"
#include "simulation.h"
#include "statistics.h"

// const float p1 = 0.3;
// const float p2 = 0.3;
// const float p3 = 0.4;
// const float pb = 0.3;
// const int epochs = 36000;

int main(int argc, char** argv) {
    // Parse arguments
    Args args(argc, argv);
    auto& settings = Settings::GetInstance();
    auto& statistics = Statistics::GetInstance();

    statistics.setLogFile(settings.getLogFilePath());
    statistics.setEpochs(settings.getEpochs());

    // Run simulation
    Simulation sim;
    sim.run(settings.getEpochs());

    // Log
    statistics.log();
    return 0;
}
