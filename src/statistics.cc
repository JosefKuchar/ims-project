#include "statistics.h"
#include "settings.h"

Statistics& Statistics::GetInstance() {
    static Statistics instance;
    return instance;
};

Statistics::Statistics() {
    mVehicleCount = 0;
    mPcu = 0;
    mLogFilePath = std::filesystem::path("log.csv");
}

void Statistics::setLogFile(std::string path) {
    mLogFilePath = std::filesystem::path(path);
};

void Statistics::setEpochs(int epochs) {
    mEpochs = epochs;
}

void Statistics::addVehicle(std::shared_ptr<CellMeta> cell) {
    mVehicleCount += 1;
    mPcu += cell->getVehiclePcu();
}

void Statistics::log() {
    auto& s = Settings::GetInstance();
    bool exists = std::filesystem::exists(mLogFilePath);

    std::ofstream logFile;
    logFile.open(mLogFilePath, std::ios::app);

    if (!exists) {
        logFile << "epochs,vehicle_count,pcu,diameter,approach_count\n";
    }

    logFile << std::to_string(mEpochs) << "," << std::to_string(mVehicleCount) << ","
            << std::to_string(mPcu) << ","
            << std::to_string((int)(s.getRoundaboutLength() / 3.14 + 1)) << ","
            << std::to_string(s.getApproachCount()) << "\n";

    logFile.close();
}
