#pragma once

#include <memory>
#include <random>
#include "settings.h"

enum class CellType { Road, Motorcycle, PersonalVehicle, Van, Bus, OtherLarge };

class CellMeta {
   private:
    int mNas;
    int mSpeed;  // cells/second
    int mExitIndex;
    CellType mType;

   public:
    CellMeta(CellType type, int approachIndex);
    int getNas() const;
    int getSpeed() const;
    CellType getType() const;
    void setSpeed(int speed);
    int getVehicleLength();
    float getVehiclePcu();
    int getExitIndex() const;
    void newNas();
};

class Cell {
   private:
    CellType mType;
    std::shared_ptr<CellMeta> mMeta;

   public:
    Cell(std::shared_ptr<CellMeta> meta);
    Cell();
    void print();
    CellType getType();
    std::shared_ptr<CellMeta> getMeta();
};
