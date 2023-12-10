#include "roundabout.h"
#include <iostream>
#include "settings.h"

Roundabout::Roundabout() {
    auto& s = Settings::GetInstance();
    mRoad = std::vector<Cell>(s.getRoundaboutLength());
    mNextRoad = std::vector<Cell>(s.getRoundaboutLength());
    for (int i = 0; i < s.getApproachCount(); i++) {
        mOutgoing.push_back(Outgoing((s.getRoundaboutLength() / s.getApproachCount()) * i));
    };
}

void Roundabout::print() {
    for (size_t i = 0; i < mRoad.size(); i++) {
        bool found = false;
        for (size_t j = 0; j < mOutgoing.size(); j++) {
            if ((size_t)mOutgoing[j].getJoinIndex() == i) {
                std::cout << "^";
                found = true;
                break;
            } else if ((size_t)mOutgoing[j].getJoinIndex() + 1 == i) {
                std::cout << "v";
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    Road::print();
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

        if (meta->getSpeed() < 2) {
            meta->setSpeed(meta->getSpeed() + 1);
        } else if (meta->getSpeed() < 14) {
            meta->setSpeed(meta->getSpeed() + 2);
        } else {
            const float r = s.getRandomFloat();
            if (r < 0.3) {
                meta->setSpeed(meta->getSpeed() + 1);
            } else if (r < 0.6) {
                meta->setSpeed(meta->getSpeed() - 1);
            }
            if (meta->getSpeed() > 16) {
                meta->setSpeed(16);
            }
        }

        if (gap < meta->getSpeed()) {
            meta->setSpeed(gap * (2.0 / 3.0));
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
    auto& s = Settings::GetInstance();
    auto spaceBehind = getSpaceBehind(joinIndex);
    auto spaceAhead = getSpaceAhead(joinIndex);
    auto vehicleLength = meta->getVehicleLength();

    if ((spaceBehind >= meta->getNas() || s.getRandomFloat() < 0.35) &&
        vehicleLength <= spaceAhead) {
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
