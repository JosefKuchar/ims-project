#include "cell.h"
#include <iostream>

Cell::Cell() {
    mType = CellType::Road;
    mMeta = nullptr;
};

Cell::Cell(CellType type, std::shared_ptr<CellMeta> meta) {
    mType = type;
    mMeta = meta;
};

std::shared_ptr<CellMeta> Cell::getMeta() {
    return mMeta;
}

CellType Cell::getType() {
    return mType;
}

CellMeta::CellMeta() {
    auto& s = Settings::GetInstance();

    mSpeed = s.getInitialSpeed();
    mNas = s.getRandomNas();
}

int CellMeta::getNas() const {
    return mNas;
}

int CellMeta::getSpeed() const {
    return mSpeed;
}

void CellMeta::setSpeed(int speed) {
    mSpeed = speed;
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
