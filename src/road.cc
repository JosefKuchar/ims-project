#include "road.h"
#include <iostream>
#include "statistics.h"

Road::Road(){};

void Road::print() {
    for (auto& cell : mRoad) {
        cell.print();
    }
    std::cout << std::endl;
}

void Road::applyUpdate() {
    // Copy the next road to the current road
    for (size_t i = 0; i < mRoad.size(); i++) {
        mRoad[i] = mNextRoad[i];
    }
    // Set the next road to be empty
    mNextRoad = std::vector<Cell>(mRoad.size());
}

void Road::update() {
    auto& s = Settings::GetInstance();
    auto& stats = Statistics::GetInstance();
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

        if (meta->getSpeed() > getDistanceToEnd(i) || getDistanceToEnd(i) == 0) {
            stats.addVehicle(meta);
            // This will not update the car so the car will disappear
            continue;
        }

        // Update car
        for (size_t j = i; j < mRoad.size() && mRoad[j].getMeta() == meta; j++) {
            mNextRoad[j + meta->getSpeed()] = mRoad[j];
        }
    }
}

int Road::getDistanceToEnd(int index) {
    auto meta = mRoad[index].getMeta();
    int j = 0;
    while ((size_t)(index + j) < mRoad.size() && meta == mRoad[index + j].getMeta()) {
        j++;
    }
    return mRoad.size() - (index + j);
}

int Road::getFreeSpaceAfter(int index) {
    auto meta = mRoad[index].getMeta();
    if (meta != nullptr) {
        while ((size_t)index < mRoad.size() && mRoad[index].getMeta() == meta) {
            index++;
        }
    }
    for (size_t i = index; i < mRoad.size(); i++) {
        if (mRoad[i].getType() != CellType::Road) {
            return i - index;
        }
    }
    // TODO: possible off by 1
    return mRoad.size() - index;
}

int Road::getFreeSpace(int index) {
    for (size_t i = index; i < mRoad.size(); i++) {
        if (mRoad[i].getType() != CellType::Road) {
            return i - index;
        }
    }
    // TODO: possible off by 1
    return mRoad.size() - index;
}
