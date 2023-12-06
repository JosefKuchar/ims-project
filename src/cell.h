#pragma once

#include <memory>
#include <random>
#include "settings.h"

enum class CellType { Road, Motorcycle, PersonalVehicle, Van, Bus, OtherLarge };

class CellMeta {
   private:
    int mNas;
    int mSpeed;  // cells/second

   public:
    CellMeta();
    int getNas() const;
    int getSpeed() const;
    void setSpeed(int speed);
};

class Cell {
   private:
    CellType mType;
    std::shared_ptr<CellMeta> mMeta;

   public:
    Cell(CellType type, std::shared_ptr<CellMeta> meta);
    Cell();
    void print();
    CellType getType();
    std::shared_ptr<CellMeta> getMeta();
};
