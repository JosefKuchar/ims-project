#pragma once

#include <random>
#include <vector>
#include "approach.h"
#include "roundabout.h"

class Simulation {
    std::vector<Approach> mIncoming;
    std::shared_ptr<Roundabout> mRoundabout;

   public:
    Simulation();

    void update();

    void print();

    void run(int num_iters);
};
