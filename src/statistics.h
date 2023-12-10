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

    /**
     * Get the singleton instance of the Statistics class.
     */
    static Statistics& GetInstance();

    /**
     * Log the current state of the simulation.
     */
    void log();

    /**
     * Set the log file path.
     * @param path The path to the log file.
     */
    void setLogFile(std::string path);

    /**
     * Set the number of epochs.
     */
    void setEpochs(int epochs);

    /**
     * Add a vehicle to the statistics.
     */
    void addVehicle(std::shared_ptr<CellMeta> cell);
};
