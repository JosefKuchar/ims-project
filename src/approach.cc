#include "approach.h"
#include <iostream>
#include <stdexcept>
#include "settings.h"

Approach::Approach(int joinIndex, std::shared_ptr<Roundabout> roundabout) {
    auto& s = Settings::GetInstance();
    mRoad = std::vector<Cell>(s.getApproachLength());
    mNextRoad = std::vector<Cell>(s.getApproachLength());
    mJoinIndex = joinIndex;
    mRoundabout = roundabout;
}

int getTypeLength(CellType type) {
    switch (type) {
        case CellType::Motorcycle:
            return 3;
        case CellType::PersonalVehicle:
            return 5;
        case CellType::Van:
            return 7;
        case CellType::Bus:
            return 10;
        case CellType::OtherLarge:
            return 13;
        default:
            throw std::runtime_error("Unreachable");
    }
}

void Approach::update() {
    auto& s = Settings::GetInstance();
    auto lastMeta = std::shared_ptr<CellMeta>(nullptr);
    for (int i = 0; i < mRoad.size(); i++) {
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
            auto spaceBehind = mRoundabout->getSpaceBehind(mJoinIndex);
            auto spaceAhead = mRoundabout->getSpaceAhead(mJoinIndex);

            // Error in paper
            if (spaceBehind >= meta->getNas() && getTypeLength(cell.getType()) <= spaceAhead) {
                std::cout << "TODO Move to roundabout" << std::endl;
                // for (int k = j; k < mRoad.size() && mRoad[k].getMeta() == meta; k++) {
                //     mRoundabout->mRoad[mIndex + k - j] = mRoad[k];
                // }
            }
        }

        if (meta->getSpeed() > getDistanceToEnd(i)) {
            meta->setSpeed(getDistanceToEnd(i));
        }

        // Update car
        for (int j = i; j < mRoad.size() && mRoad[j].getMeta() == meta; j++) {
            mNextRoad[j + meta->getSpeed()] = mRoad[j];
        }
    }
}

CellType generateType() {
    auto& s = Settings::GetInstance();
    float r = s.getRandomFloat();

    if (r < s.getMotorcycleP()) {
        return CellType::Motorcycle;
    } else if (r < s.getMotorcycleP() + s.getCarP()) {
        return CellType::PersonalVehicle;
    } else if (r < s.getMotorcycleP() + s.getCarP() + s.getVanP()) {
        return CellType::Van;
    } else if (r < s.getMotorcycleP() + s.getCarP() + s.getVanP() + s.getBusP()) {
        return CellType::Bus;
    } else {
        return CellType::OtherLarge;
    }
}

void Approach::trySpawnVehicle() {
    auto meta = std::make_shared<CellMeta>(CellMeta());
    auto vehicleType = generateType();
    auto vehicleLength = getTypeLength(vehicleType);
    auto freeSpace = getFreeSpace(0);

    if (meta->getNas() + vehicleLength > freeSpace) {
        return;
    }

    for (int j = 0; j < vehicleLength; j++) {
        mRoad[j] = Cell(vehicleType, meta);
    }
}

int Approach::getFreeSpace(int index) {
    for (int i = index; i < mRoad.size(); i++) {
        if (mRoad[i].getType() != CellType::Road) {
            return i - index;
        }
    }
    // TODO: possible off by 1
    return mRoad.size() - index;
}

int Approach::getFreeSpaceAfter(int index) {
    auto meta = mRoad[index].getMeta();
    if (meta != nullptr) {
        while (index < mRoad.size() && mRoad[index].getMeta() == meta) {
            index++;
        }
    }
    for (int i = index; i < mRoad.size(); i++) {
        if (mRoad[i].getType() != CellType::Road) {
            return i - index;
        }
    }
    // TODO: possible off by 1
    return mRoad.size() - index;
}

int Approach::getDistanceToEnd(int index) {
    auto meta = mRoad[index].getMeta();
    int j = 0;
    while (index + j < mRoad.size() && meta == mRoad[index + j].getMeta()) {
        j++;
    }
    return mRoad.size() - (index + j);
}
