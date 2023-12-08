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

void Approach::update() {
    auto& s = Settings::GetInstance();
    auto lastMeta = std::shared_ptr<CellMeta>(nullptr);
    for (size_t i = 0; i < mRoad.size(); i++) {
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
            if (s.getRandomFloat() < 0.1) {
                meta->setSpeed(meta->getSpeed() - 1);
            }
        }

        // FIXME Move this to roundabout logic
        // If we are at the end of the approach
        if (getDistanceToEnd(i) == 0) {
            meta->newNas();
            if (mRoundabout->trySpawnVehicle(meta, mJoinIndex)) {
                // Car dissapears from this road
                continue;
            }
        }

        if (meta->getSpeed() > getDistanceToEnd(i)) {
            meta->setSpeed(getDistanceToEnd(i));
        }

        // Update car
        for (size_t j = i; j < mRoad.size() && mRoad[j].getMeta() == meta; j++) {
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

void Approach::trySpawnVehicle(int approachIndex) {
    auto vehicleType = generateType();
    auto meta = std::make_shared<CellMeta>(CellMeta(vehicleType, approachIndex));
    auto vehicleLength = meta->getVehicleLength();
    auto freeSpace = getFreeSpace(0);

    if (meta->getNas() + vehicleLength > freeSpace) {
        return;
    }

    for (int j = 0; j < vehicleLength; j++) {
        mRoad[j] = Cell(meta);
    }
}
