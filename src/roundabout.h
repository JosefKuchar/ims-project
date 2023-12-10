#pragma once

#include "outgoing.h"
#include "road.h"

class Roundabout : public Road {
   private:
    std::vector<Outgoing> mOutgoing;

   public:
    /**
     * Construct a new Roundabout object.
     */
    Roundabout();

    /**
     * Update the roundabout.
     */
    void update();

    /**
     * Print the roundabout.
     */
    void print();

    /**
     * Apply the update to the roundabout.
     */
    void applyUpdate();

    /**
     * Get the space ahead of a given index.
     */
    int getSpaceAhead(int index);

    /**
     * Get the space behind a given index.
     */
    int getSpaceBehind(int index);

    /**
     * Get the distance to the end of the roundabout.
     */
    int getDistanceToEnd(int index);

    /**
     * Get the free space after a given index.
     */
    int getFreeSpaceAfter(int index);

    /**
     * Try to spawn a vehicle.
     */
    bool trySpawnVehicle(std::shared_ptr<CellMeta> meta, int joinIndex);
    std::vector<Outgoing>& getOutgoing();
};
