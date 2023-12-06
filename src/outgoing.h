#pragma once

#include "road.h"

class Outgoing : public Road {
   private:
    int mJoinIndex;

   public:
    Outgoing(int joinIndex);
    bool trySpawnVehicle(std::shared_ptr<CellMeta> meta);
    int getJoinIndex() const;
};
