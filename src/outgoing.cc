#include "outgoing.h"

Outgoing::Outgoing(int joinIndex) : mJoinIndex(joinIndex) {
    auto& s = Settings::GetInstance();
    mRoad = std::vector<Cell>(s.getApproachLength());
    mNextRoad = std::vector<Cell>(s.getApproachLength());
};

bool Outgoing::trySpawnVehicle(std::shared_ptr<CellMeta> meta){};
