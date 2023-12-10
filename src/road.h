#pragma once

#include <vector>
#include "cell.h"

class Road {
   protected:
    std::vector<Cell> mRoad;
    std::vector<Cell> mNextRoad;

   public:
    /**
     * Construct a new Road object.
     */
    Road();

    /**
     * Print the road.
     */
    void print();

    /**
     * Update the road.
     */
    void update();

    /**
     * Apply the update to the road.
     */
    void applyUpdate();

    /**
     * Get the space ahead of a given index.
     */
    int getFreeSpace(int index);

    /**
     * Get distance to the end of the road.
     */
    int getDistanceToEnd(int index);

    /**
     * Get the free space after a given index.
     */
    int getFreeSpaceAfter(int startIndex);
};
