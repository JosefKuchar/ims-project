#pragma once

#include "road.h"

class Roundabout : public Road {
   public:
    Roundabout();
    int getSpaceAhead(int index);
    int getSpaceBehind(int index);
};
