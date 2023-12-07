#pragma once

#include <random>

class Settings {
   private:
    Settings();

    // Settings
    int mMinNas;
    int mMaxNas;
    int mNasMean;
    int mNasDeviation;
    int mApproachCount;
    int mApproachLength;
    int mRoundaboutLength;
    int mInitialSpeed;
    float mMotorcycleP;
    float mCarP;
    float mVanP;
    float mBusP;

    std::random_device mRd;
    std::mt19937 mGen;
    std::normal_distribution<> mNasDist;
    std::uniform_real_distribution<> mUniformDist;

   public:
    // Prevent certain operations on singleton
    Settings(const Settings&) = delete;
    void operator=(const Settings&) = delete;
    Settings(const Settings&&) = delete;
    void operator=(const Settings&&) = delete;

    // Getters
    int getMinNas() const;
    int getMaxNas() const;
    int getNasMean() const;
    int getNasDeviation() const;
    int getApproachCount() const;
    int getApproachLength() const;
    int getRoundaboutLength() const;
    int getInitialSpeed() const;
    float getMotorcycleP() const;
    float getCarP() const;
    float getVanP() const;
    float getBusP() const;

    int getRandomNas();
    float getRandomFloat();
    int getExitIndex(int approachIndex);

    static Settings& GetInstance();

    int vehicleCount = 0;
};
