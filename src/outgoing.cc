#include "outgoing.h"

Outgoing::Outgoing(int joinIndex) : mJoinIndex(joinIndex) {
    auto& s = Settings::GetInstance();
    mRoad = std::vector<Cell>(s.getApproachLength());
    mNextRoad = std::vector<Cell>(s.getApproachLength());
};

bool Outgoing::trySpawnVehicle(std::shared_ptr<CellMeta> meta) {
    auto vehicleLength = meta->getVehicleLength();
    auto freeSpace = getFreeSpace(0);
    if (vehicleLength > freeSpace) {
        return false;
    }
    for (int j = 0; j < vehicleLength; j++) {
        mRoad[j] = Cell(meta);
    }
    return true;
}

int Outgoing::getJoinIndex() const {
    return mJoinIndex;
}
