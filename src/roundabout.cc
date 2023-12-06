#include "roundabout.h"
#include "settings.h"

Roundabout::Roundabout() {
    auto& s = Settings::GetInstance();
    mRoad = std::vector<Cell>(s.getRoundaboutLength());
    mNextRoad = std::vector<Cell>(s.getRoundaboutLength());
}

int Roundabout::getSpaceAhead(int index) {
    for (int i = index; i < mRoad.size(); i++) {
        if (mRoad[i].getType() != CellType::Road) {
            return i - index;
        }
    }
    return mRoad.size() - index;
}

int Roundabout::getSpaceBehind(int index) {
    int total = 0;

    while (total < mRoad.size()) {
        if (mRoad[index].getType() != CellType::Road) {
            return total;
        }
        index = index - 1;
        total += 1;
        if (index < 0) {
            index = mRoad.size() - 1;
        }
    }

    return total;
}
