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
    void applyUpdate();
};
