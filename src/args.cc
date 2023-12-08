#include "args.h"
#include <getopt.h>
#include <iostream>
#include <string>
#include "settings.h"

enum class Arg {
    MinNas = 'a',
    MaxNas = 'b',
    MeanNas = 'c',
    DeviationNas = 'd',
    ApproachCount = 'e',
    ApproachLength = 'l',
    RoundaboutDiameter = 'r',
    InitialSpeed = 's',
    MotorcycleP = 'i',
    CarP = 'j',
    VanP = 'k',
    BusP = 'q',
    Epochs = 'm',
    LogFile = 'n',
    Help = 'h',
    Verbose = 'v'
};

static struct option options[] = {
    {"min-nas", required_argument, 0, (char)Arg::MinNas},
    {"max-nas", required_argument, 0, (char)Arg::MaxNas},
    {"mean-nas", required_argument, 0, (char)Arg::MeanNas},
    {"deviation-nas", required_argument, 0, (char)Arg::DeviationNas},
    {"approach-count", required_argument, 0, (char)Arg::ApproachCount},
    {"approach-length", required_argument, 0, (char)Arg::ApproachLength},
    {"roundabout-diameter", required_argument, 0, (char)Arg::RoundaboutDiameter},
    {"initial-speed", required_argument, 0, (char)Arg::InitialSpeed},
    {"motorcycle-p", required_argument, 0, (char)Arg::MotorcycleP},
    {"car-p", required_argument, 0, (char)Arg::CarP},
    {"van-p", required_argument, 0, (char)Arg::VanP},
    {"bus-p", required_argument, 0, (char)Arg::BusP},
    {"epochs", required_argument, 0, (char)Arg::Epochs},
    {"log-file", required_argument, 0, (char)Arg::LogFile},
    {"verbose", no_argument, 0, (char)Arg::Verbose},
    {"help", no_argument, 0, (char)Arg::Help},
    {0, 0, 0, 0}};

std::string argDesc[] = {"Minimum number of NAS per vehicle [default: 16]",
                         "Maximum number of NAS per vehicle [default: 26]",
                         "Mean number of NAS per vehicle  [default: 20]",
                         "Standard deviation of number of NAS per vehicle [default: 2]",
                         "Number of approaches [default: 4]",
                         "Length of each approach [default: 100]",
                         "Diameter of the roundabout [default: 30]",
                         "Initial speed of vehicles [default: 16]",
                         "Probability of motorcycle [default: 0.02]",
                         "Probability of car [default: 0.78]",
                         "Probability of van [default: 0.11]",
                         "Probability of bus [default: 0.06]",
                         "Number of epochs (1 epoch = 1 second) [default: 3600]",
                         "Path to log file [default: log.csv]",
                         "Print verbose (each step of simulation) [default: false]",
                         "Print help"};

Args::Args(int argc, char** argv) {
    auto& s = Settings::GetInstance();
    try {
        while (true) {
            int optIndex = 0;
            int c = getopt_long(argc, argv, ":a:b:c:d:e:l:r:s:i:j:k:q:m:nhv", options, &optIndex);

            if (c == -1) {
                break;
            }

            switch (c) {
                case (char)Arg::Help:
                    printHelp();
                    exit(0);
                    break;
                case (char)Arg::Verbose:
                    s.setVerbose(true);
                    break;
                case (char)Arg::MinNas:
                    s.setMinNas(std::stoi(optarg));
                    break;
                case (char)Arg::MaxNas:
                    s.setMaxNas(std::stoi(optarg));
                    break;
                case (char)Arg::MeanNas:
                    s.setNasMean(std::stoi(optarg));
                    break;
                case (char)Arg::DeviationNas:
                    s.setNasDeviation(std::stoi(optarg));
                    break;
                case (char)Arg::ApproachCount:
                    s.setApproachCount(std::stoi(optarg));
                    break;
                case (char)Arg::ApproachLength:
                    s.setApproachLength(std::stoi(optarg));
                    break;
                case (char)Arg::RoundaboutDiameter:
                    s.setRoundaboutLength(std::stoi(optarg) * 3.14);
                    break;
                case (char)Arg::InitialSpeed:
                    s.setInitialSpeed(std::stoi(optarg));
                    break;
                case (char)Arg::MotorcycleP:
                    s.setMotorcycleP(std::stof(optarg));
                    break;
                case (char)Arg::CarP:
                    s.setCarP(std::stof(optarg));
                    break;
                case (char)Arg::VanP:
                    s.setVanP(std::stof(optarg));
                    break;
                case (char)Arg::BusP:
                    s.setBusP(std::stof(optarg));
                    break;
                case (char)Arg::Epochs:
                    s.setEpochs(std::stoi(optarg));
                    break;
                case (char)Arg::LogFile:
                    s.setLogFile(optarg);
                    break;
                case ':':
                    std::cout << "Missing argument for option " << options[optIndex].name
                              << std::endl;
                    printHelp();
                    exit(1);
                    break;
                default:
                    std::cout << std::endl;
                    printHelp();
                    exit(1);
                    break;
            }
        }
    } catch (std::invalid_argument& e) {
        std::cout << "Invalid value" << std::endl;
        printHelp();
        exit(1);
    } catch (std::out_of_range& e) {
        std::cout << "Value out of range" << std::endl;
        printHelp();
        exit(1);
    }

    validate();
};

void Args::printHelp() {
    std::cout << "Usage: ./model [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    for (size_t i = 0; i < sizeof(options) / sizeof(struct option) - 1; i++) {
        std::cout << "  --" << options[i].name << " (-" << (char)options[i].val << ") "
                  << argDesc[i] << std::endl;
    }
}

void Args::validate() {
    auto& s = Settings::GetInstance();
    if (s.getMinNas() > s.getMaxNas()) {
        std::cout << "Minimum number of NAS per vehicle cannot be greater than maximum number of "
                     "NAS per vehicle"
                  << std::endl;
        exit(1);
    }
    if (s.getApproachCount() < 1) {
        std::cout << "Number of approaches cannot be less than 1" << std::endl;
        exit(1);
    }
    if (s.getApproachLength() < 15) {
        std::cout << "Length of each approach cannot be less than 15" << std::endl;
        exit(1);
    }
    if (s.getInitialSpeed() < 1) {
        std::cout << "Initial speed of vehicles cannot be less than 1" << std::endl;
        exit(1);
    }
    if (s.getMotorcycleP() < 0 || s.getMotorcycleP() > 1) {
        std::cout << "Probability of motorcycle must be between 0 and 1" << std::endl;
        exit(1);
    }
    if (s.getCarP() < 0 || s.getCarP() > 1) {
        std::cout << "Probability of car must be between 0 and 1" << std::endl;
        exit(1);
    }
    if (s.getVanP() < 0 || s.getVanP() > 1) {
        std::cout << "Probability of van must be between 0 and 1" << std::endl;
    }
    if (s.getBusP() < 0 || s.getBusP() > 1) {
        std::cout << "Probability of bus must be between 0 and 1" << std::endl;
        exit(1);
    }
    if (s.getMotorcycleP() + s.getCarP() + s.getVanP() + s.getBusP() >= 1) {
        std::cout << "Sum of probabilities of motorcycle, car, van and bus must be less than 1"
                  << std::endl;
        exit(1);
    }
    if (s.getEpochs() < 1) {
        std::cout << "Number of epochs cannot be less than 1" << std::endl;
        exit(1);
    }
    if (s.getLogFilePath().empty()) {
        std::cout << "Log file path cannot be empty" << std::endl;
        exit(1);
    }
    if (s.getRoundaboutLength() / s.getApproachCount() < 15) {
        std::cout << "Roundabout circumference cannot be less than 15 times number of approaches"
                  << std::endl;
        exit(1);
    }
}
