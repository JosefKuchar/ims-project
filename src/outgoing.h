#pragma once

#include "road.h"

class Outgoing : public Road {
   private:
    int mJoinIndex;

   public:
    /**
     * Construct a new Outgoing object.
     */
    Outgoing(int joinIndex);

    /**
     * Update the outgoing road.
     */
    bool trySpawnVehicle(std::shared_ptr<CellMeta> meta);

    /**
     * Get the join index.
     */
    int getJoinIndex() const;
};
