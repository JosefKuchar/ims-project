#include "settings.h"

Settings& Settings::GetInstance() {
    static Settings instance;
    return instance;
};

Settings::Settings() {
    mMinNas = 16;
    mMaxNas = 26;
    mNasMean = 20;
    mNasDeviation = 2;
    mApproachCount = 4;
    mApproachLength = 100;
    mRoundaboutLength = 30 * 3.14;
    mInitialSpeed = 16;
    mMotorcycleP = 0.02;
    mCarP = 0.78;
    mVanP = 0.11;
    mBusP = 0.06;
    mLogFilePath = "log.csv";
    mEpochs = 3600;

    mGen = std::mt19937(mRd());
    mNasDist = std::normal_distribution<>(mNasMean, mNasDeviation);
    mUniformDist = std::uniform_real_distribution<>(0, 1);
}

bool Settings::getVerbose() const {
    return mVerbose;
}

int Settings::getMinNas() const {
    return mMinNas;
}

int Settings::getMaxNas() const {
    return mMaxNas;
}

int Settings::getApproachCount() const {
    return mApproachCount;
}

int Settings::getNasMean() const {
    return mNasMean;
}

int Settings::getNasDeviation() const {
    return mNasDeviation;
}

int Settings::getApproachLength() const {
    return mApproachLength;
}

int Settings::getRoundaboutLength() const {
    return mRoundaboutLength;
}

int Settings::getEpochs() const {
    return mEpochs;
}

float Settings::getMotorcycleP() const {
    return mMotorcycleP;
}

float Settings::getCarP() const {
    return mCarP;
}

float Settings::getVanP() const {
    return mVanP;
}

float Settings::getBusP() const {
    return mBusP;
}

int Settings::getInitialSpeed() const {
    return mInitialSpeed;
}

std::string Settings::getLogFilePath() const {
    return mLogFilePath;
}

void Settings::setVerbose(bool verbose) {
    mVerbose = verbose;
}

void Settings::setMinNas(int minNas) {
    mMinNas = minNas;
}

void Settings::setMaxNas(int maxNas) {
    mMaxNas = maxNas;
}

void Settings::setNasMean(int nasMean) {
    mNasMean = nasMean;
}

void Settings::setNasDeviation(int nasDeviation) {
    mNasDeviation = nasDeviation;
}

void Settings::setApproachCount(int approachCount) {
    mApproachCount = approachCount;
}

void Settings::setApproachLength(int approachLength) {
    mApproachLength = approachLength;
}

void Settings::setRoundaboutLength(int roundaboutLength) {
    mRoundaboutLength = roundaboutLength;
}

void Settings::setEpochs(int epochs) {
    mEpochs = epochs;
}

void Settings::setMotorcycleP(float motorcycleP) {
    mMotorcycleP = motorcycleP;
}

void Settings::setCarP(float carP) {
    mCarP = carP;
}

void Settings::setVanP(float vanP) {
    mVanP = vanP;
}

void Settings::setBusP(float busP) {
    mBusP = busP;
}

void Settings::setInitialSpeed(int initialSpeed) {
    mInitialSpeed = initialSpeed;
}

void Settings::setLogFile(std::string path) {
    mLogFilePath = path;
}

int Settings::getExitIndex(int approachIndex) {
    auto rn = mUniformDist(mGen);
    return (approachIndex + 1) % mApproachCount;
    // return (approachIndex + 3) % mApproachCount;
    if (rn < 0.33) {
        return (approachIndex + 1) % mApproachCount;
    } else if (rn < 0.33) {
        return (approachIndex + 2) % mApproachCount;
    } else {
        return (approachIndex + 3) % mApproachCount;
    }
}

int Settings::getRandomNas() {
    int nas = mNasDist(mGen);
    if (nas < mMinNas) {
        return mMinNas;
    } else if (nas > mMaxNas) {
        return mMaxNas;
    }
    return nas;
}

float Settings::getRandomFloat() {
    return mUniformDist(mGen);
}
