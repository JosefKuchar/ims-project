#pragma once

#include <vector>
#include "cell.h"

class Road {
   protected:
    std::vector<Cell> mRoad;
    std::vector<Cell> mNextRoad;

   public:
    Road();
    void print();
    void update();
    void applyUpdate();
    int getFreeSpace(int index);
    int getDistanceToEnd(int index);
    int getFreeSpaceAfter(int startIndex);
};
