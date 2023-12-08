#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "cell.h"

class Statistics {
   private:
    Statistics();

    float mPcu;
    int mVehicleCount;
    int mEpochs;
    std::filesystem::path mLogFilePath;

   public:
    Statistics(const Statistics&) = delete;
    void operator=(const Statistics&) = delete;
    Statistics(const Statistics&&) = delete;
    void operator=(const Statistics&&) = delete;

    static Statistics& GetInstance();
    void log();
    void setLogFile(std::string path);
    void setEpochs(int epochs);
    void addVehicle(std::shared_ptr<CellMeta> cell);
};
