#include "roundabout.h"
#include <iostream>
#include "settings.h"
#include "utils.h"

Roundabout::Roundabout() {
    auto& s = Settings::GetInstance();
    mRoad = std::vector<Cell>(s.getRoundaboutLength());
    mNextRoad = std::vector<Cell>(s.getRoundaboutLength());
    mOutgoing = std::vector<Road>(s.getApproachCount());
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

void Roundabout::update() {
    // TODO update roundabout
    for (int i = 0; i < mRoad.size(); i++) {
        mNextRoad[i] = mRoad[i];
    }

    //  Update roads
    for (auto& a : mOutgoing) {
        a.update();
    }
}

bool Roundabout::trySpawnVehicle(std::shared_ptr<CellMeta> meta, int joinIndex) {
    auto spaceBehind = getSpaceBehind(joinIndex);
    auto spaceAhead = getSpaceAhead(joinIndex);
    auto vehicleLength = meta->getVehicleLength();

    if (spaceBehind >= meta->getNas() && vehicleLength <= spaceAhead) {
        for (int i = 0; i < vehicleLength; i++) {
            mRoad[joinIndex + i] = Cell(meta);
        }
        return true;
    };

    return false;
}
