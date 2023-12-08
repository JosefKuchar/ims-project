#pragma once

#include <random>
#include <string>

class Settings {
   private:
    Settings();

    // Settings
    bool mVerbose;
    int mMinNas;
    int mMaxNas;
    int mNasMean;
    int mNasDeviation;
    int mApproachCount;
    int mApproachLength;
    int mRoundaboutLength;
    int mInitialSpeed;
    int mEpochs;
    float mMotorcycleP;
    float mCarP;
    float mVanP;
    float mBusP;

    std::string mLogFilePath;

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
    bool getVerbose() const;
    int getMinNas() const;
    int getMaxNas() const;
    int getNasMean() const;
    int getNasDeviation() const;
    int getApproachCount() const;
    int getApproachLength() const;
    int getRoundaboutLength() const;
    int getInitialSpeed() const;
    int getEpochs() const;
    float getMotorcycleP() const;
    float getCarP() const;
    float getVanP() const;
    float getBusP() const;
    std::string getLogFilePath() const;

    // Setters
    void setVerbose(bool);
    void setMinNas(int);
    void setMaxNas(int);
    void setNasMean(int);
    void setNasDeviation(int);
    void setApproachCount(int);
    void setApproachLength(int);
    void setRoundaboutLength(int);
    void setInitialSpeed(int);
    void setEpochs(int);
    void setMotorcycleP(float);
    void setCarP(float);
    void setVanP(float);
    void setBusP(float);
    void setLogFile(std::string);

    int getRandomNas();
    float getRandomFloat();
    int getExitIndex(int approachIndex);

    static Settings& GetInstance();
};
