#pragma once

#include <memory>
#include "road.h"
#include "roundabout.h"

class Approach : public Road {
    int mJoinIndex;
    std::shared_ptr<Roundabout> mRoundabout;

   public:
    Approach(int joinIndex, std::shared_ptr<Roundabout> roundabout);
    void trySpawnVehicle(int approachIndex);
    void update();
};
