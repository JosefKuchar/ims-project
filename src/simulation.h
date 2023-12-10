#pragma once

#include <random>
#include <vector>
#include "approach.h"
#include "roundabout.h"

class Simulation {
    std::vector<Approach> mIncoming;
    std::shared_ptr<Roundabout> mRoundabout;

   public:
    /**
     * Construct a new Simulation object.
     */
    Simulation();

    /**
     * Update the simulation.
     */
    void update();

    /**
     * Print the simulation state.
     */
    void print();

    /**
     * Run the simulation for a number of iterations.
     * @param num_iters The number of iterations to run the simulation for.
     */
    void run(int num_iters);
};
