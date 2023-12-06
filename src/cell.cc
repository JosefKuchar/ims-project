#include "cell.h"
#include <iostream>

Cell::Cell() {
    mType = CellType::Road;
    mMeta = nullptr;
};

Cell::Cell(std::shared_ptr<CellMeta> meta) {
    mType = meta->getType();
    mMeta = meta;
};

std::shared_ptr<CellMeta> Cell::getMeta() {
    return mMeta;
}

CellType Cell::getType() {
    return mType;
}

CellMeta::CellMeta(CellType type, int approachIndex) {
    auto& s = Settings::GetInstance();

    mSpeed = s.getInitialSpeed();
    mNas = s.getRandomNas();
    mExitIndex = s.getExitIndex(approachIndex);
    mType = type;
}

int CellMeta::getNas() const {
    return mNas;
}

int CellMeta::getSpeed() const {
    return mSpeed;
}

CellType CellMeta::getType() const {
    return mType;
}

int CellMeta::getExitIndex() const {
    return mExitIndex;
}

void CellMeta::setSpeed(int speed) {
    mSpeed = speed;
}

int CellMeta::getVehicleLength() {
    switch (mType) {
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

void Cell::print() {
    switch (mType) {
        case CellType::Road:
            std::cout << ".";
            break;
        case CellType::Motorcycle:
            std::cout << "M";
            break;
        case CellType::PersonalVehicle:
            std::cout << "P";
            break;
        case CellType::Van:
            std::cout << "V";
            break;
        case CellType::Bus:
            std::cout << "B";
            break;
        case CellType::OtherLarge:
            std::cout << "L";
            break;
        default:
            throw std::runtime_error("Invalid cell type");
    }
}
