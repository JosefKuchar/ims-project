#include "roundabout.h"
#include <iostream>
#include "settings.h"
#include "utils.h"

Roundabout::Roundabout() {
    auto& s = Settings::GetInstance();
    mRoad = std::vector<Cell>(s.getRoundaboutLength());
    mNextRoad = std::vector<Cell>(s.getRoundaboutLength());
    for (int i = 0; i < s.getApproachCount(); i++) {
        mOutgoing.push_back(Outgoing((s.getRoundaboutLength() / s.getApproachCount()) * i));
    };
}

int Roundabout::getSpaceAhead(int index) {
    for (size_t i = index; i < mRoad.size(); i++) {
        if (mRoad[i].getType() != CellType::Road) {
            return i - index;
        }
    }
    return mRoad.size() - index;
}

int Roundabout::getDistanceToEnd(int index) {
    auto meta = mRoad[index].getMeta();
    int distance = 0;
    if (meta != nullptr) {
        while (mRoad[index].getMeta() == meta) {
            index++;
            index %= mRoad.size();
        }
    }
    int outgoingIndex = mOutgoing[meta->getExitIndex()].getJoinIndex();
    for (size_t i = index;; i++, i %= mRoad.size()) {
        if (i == (size_t)outgoingIndex) {
            return distance;
        }
        distance++;
    }
}

int Roundabout::getSpaceBehind(int index) {
    int total = 0;

    while ((size_t)total < mRoad.size()) {
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
    auto& s = Settings::GetInstance();
    auto lastMeta = mRoad[0].getMeta();
    int offset = 0;
    for (size_t i = 0; i < mRoad.size(); i++) {
        if (mRoad[i].getMeta() == nullptr) {
            break;
        }
        if (lastMeta != mRoad[i].getMeta()) {
            break;
        }
        offset++;
        lastMeta = mRoad[i].getMeta();
    }
    lastMeta = nullptr;
    for (size_t k = 0; k < mRoad.size(); k++) {
        int i = (k + offset) % mRoad.size();
        auto& cell = mRoad[i];
        if (cell.getType() == CellType::Road) {
            lastMeta = nullptr;
            continue;
        }
        auto meta = cell.getMeta();
        if (meta == lastMeta) {
            continue;
        }
        lastMeta = meta;
        int gap = getFreeSpaceAfter(i);

        if (gap < meta->getSpeed()) {
            meta->setSpeed(gap * (2.0 / 3.0));
        } else {
            // TODO: Hack
            meta->setSpeed(meta->getSpeed() + 2);
            if (meta->getSpeed() > 14) {
                meta->setSpeed(14);
            }
            // Hack 2
            gap = getFreeSpaceAfter(i);
            if (gap < meta->getSpeed()) {
                meta->setSpeed(gap);
            }
        }

        if (meta->getSpeed() > 0) {
            // TODO use settings prob
            if (s.getRandomFloat() < 0.3) {
                meta->setSpeed(meta->getSpeed() - 1);
            }
        }

        // FIXME Move this to roundabout logic
        // If we are at the end of the approach
        if (getDistanceToEnd(i) == 0) {
            if (mOutgoing[meta->getExitIndex()].trySpawnVehicle(meta)) {
                // Car dissapears from this road
                continue;
            }
        }

        if (meta->getSpeed() > getDistanceToEnd(i)) {
            meta->setSpeed(getDistanceToEnd(i));
        }

        // Update car
        for (size_t j = i; mRoad[j].getMeta() == meta; j++, j %= mRoad.size()) {
            mNextRoad[(j + meta->getSpeed()) % mRoad.size()] = mRoad[j % mRoad.size()];
        }
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
        for (size_t i = 0; i < (size_t)vehicleLength; i++) {
            mRoad[joinIndex + i] = Cell(meta);
        }
        return true;
    };

    return false;
}

int Roundabout::getFreeSpaceAfter(int index) {
    auto meta = mRoad[index].getMeta();
    if (meta != nullptr) {
        while (mRoad[index].getMeta() == meta) {
            index++;
            index %= mRoad.size();
        }
    }
    int total = 0;
    for (size_t i = 0; i < mRoad.size(); i++) {
        if (mRoad[(i + index) % mRoad.size()].getType() != CellType::Road) {
            break;
        }
        total++;
    }
    return total;
}

std::vector<Outgoing>& Roundabout::getOutgoing() {
    return mOutgoing;
};

void Roundabout::applyUpdate() {
    Road::applyUpdate();
    for (auto& a : mOutgoing) {
        a.applyUpdate();
    }
}
