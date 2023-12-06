#pragma once

#include "road.h"

class Roundabout : public Road {
   private:
    std::vector<Road> mOutgoing;

   public:
    Roundabout();
    void update();
    int getSpaceAhead(int index);
    int getSpaceBehind(int index);
    bool trySpawnVehicle(std::shared_ptr<CellMeta> meta, int joinIndex);
};
