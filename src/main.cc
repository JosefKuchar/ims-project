#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include "args.h"
#include "simulation.h"
#include "statistics.h"

int main(int argc, char** argv) {
    // Parse arguments
    Args args(argc, argv);
    auto& settings = Settings::GetInstance();
    // Setup statistics
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
