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
    mRoundaboutLength = 15 * 2 * 3.14;
    mInitialSpeed = 14;
    mMotorcycleP = 0.02;
    mCarP = 0.78;
    mVanP = 0.11;
    mBusP = 0.06;

    mGen = std::mt19937(mRd());
    mNasDist = std::normal_distribution<>(mNasMean, mNasDeviation);
    mUniformDist = std::uniform_real_distribution<>(0, 1);
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

int Settings::getExitIndex(int approachIndex) {
    auto rn = mUniformDist(mGen);
    if (rn < 0.33) {
        return (approachIndex + 1) % mApproachCount;
    } else if (rn < 0.66) {
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
