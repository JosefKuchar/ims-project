#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>

enum class CellType { Road, Motorcycle, PersonalVehicle, Van, Bus, OtherLarge };

const float p1 = 0.3;
const float p2 = 0.3;
const float p3 = 0.4;
const float pb = 0.3;
const int epochs = 36000;
const int approach_length = 200;
const int xMin = 16;
const int xMax = 26;
const int mean = 20;
const int deviation = 2;
const int num_approaches = 4;
const int initial_speed = 14;

const float motorcyle_chance = 0.02;
const float personal_vehicle_chance = 0.78;
const float van_chance = 0.11;
const float bus_chance = 0.06;

CellType generateType(std::mt19937& gen) {
    std::uniform_real_distribution<> dis(0, 1);
    float r = dis(gen);

    if (r < motorcyle_chance) {
        return CellType::Motorcycle;
    } else if (r < motorcyle_chance + personal_vehicle_chance) {
        return CellType::PersonalVehicle;
    } else if (r < motorcyle_chance + personal_vehicle_chance + van_chance) {
        return CellType::Van;
    } else if (r < motorcyle_chance + personal_vehicle_chance + van_chance + bus_chance) {
        return CellType::Bus;
    } else {
        return CellType::OtherLarge;
    }
}

int getTypeLength(CellType type) {
    switch (type) {
        case CellType::Motorcycle:
            return 3;
        case CellType::PersonalVehicle:
            return 5;
        case CellType::Van:
            return 7;
        case CellType::Bus:
            return 10;
        case CellType::OtherLarge:
            return 13;
        default:
            throw std::runtime_error("Unreachable");
    }
}

void printCellType(CellType type) {
    switch (type) {
        case CellType::Road:
            std::cout << ".";
            break;
        case CellType::Motorcycle:
            std::cout << "M";
            break;
        case CellType::PersonalVehicle:
            std::cout << "P";
            break;
        case CellType::Van:
            std::cout << "V";
            break;
        case CellType::Bus:
            std::cout << "B";
            break;
        case CellType::OtherLarge:
            std::cout << "L";
            break;
        default:
            throw std::runtime_error("Unreachable");
    }
}

struct CellMeta {
    int mNas;
    int mSpeed;  // cells/second

    CellMeta(std::normal_distribution<>& d, std::mt19937& gen) {
        mSpeed = 14;
        mNas = d(gen);
        if (mNas < xMin) {
            mNas = xMin;
        } else if (mNas > xMax) {
            mNas = xMax;
        }
    }
};

struct Cell {
    CellType mType;
    std::shared_ptr<CellMeta> mMeta;

   public:
    Cell(CellType type, std::shared_ptr<CellMeta> meta) {
        mType = type;
        mMeta = meta;
    };
    Cell() {
        mType = CellType::Road;
        mMeta = nullptr;
    };
    CellType getType() { return mType; }
};

struct Roundabout {
    std::array<Cell, approach_length> mRoad;

    int getSpaceAhead(int index) {
        for (int i = index; i < approach_length; i++) {
            if (mRoad[i].mType != CellType::Road) {
                return i - index;
            }
        }
        return approach_length - index;
    }

    int getSpaceBehind(int index) {
        int total = 0;

        while (total < approach_length) {
            if (mRoad[index].mType != CellType::Road) {
                return total;
            }
            index = index - 1;
            total += 1;
            if (index < 0) {
                index = approach_length - 1;
            }
        }

        return total;
    }

    std::tuple<int, int> getSpace(int index) {
        return std::tuple<int, int>(getSpaceBehind(index), getSpaceAhead(index));
    }
};

struct Approach {
    std::array<Cell, approach_length> mRoad;
    bool mIsJoining;
    int mIndex;

    Approach(bool isJoining, int index) : mIsJoining(isJoining), mIndex(index){};
    Approach() {
        mIsJoining = false;
        mIndex = 0;
        mRoad = {};
    };

    int freeSpace(int index) {
        auto& meta = mRoad[index].mMeta;
        if (meta != nullptr) {
            while (index < approach_length && mRoad[index].mMeta == meta) {
                index++;
            }
        }
        for (int i = index; i < approach_length; i++) {
            if (mRoad[i].mType != CellType::Road) {
                return i - index;
            }
        }
        // TODO: possible off by 1
        return approach_length - index;
    }

    int distanceFromEnd(int index) {
        auto& meta = mRoad[index].mMeta;
        int j = 0;
        while (index + j < approach_length && meta == mRoad[index + j].mMeta) {
            j++;
        }
        return approach_length - (index + j);
    }
};

class Simulation {
    std::array<Approach, num_approaches> approaches;
    std::array<Approach, num_approaches> leaves;
    Roundabout mRoundabout;
    std::random_device mRd;
    std::mt19937 mGen;
    std::normal_distribution<> mNasD;

   public:
    Simulation() {
        mGen = std::mt19937(mRd());
        mNasD = std::normal_distribution<>(mean, deviation);

        for (int i = 0; i < num_approaches; i++) {
            approaches[i] = Approach(true, (approach_length / num_approaches) * i);
            leaves[i] = Approach(false, (approach_length / num_approaches) * i);
        };
    }

    void update() {
        std::array<Approach, num_approaches> new_approaches;
        std::array<Approach, num_approaches> new_leaves;
        std::array<Cell, approach_length> new_roundabout_road;

        for (int i = 0; i < num_approaches; i++) {
            new_approaches[i] = Approach(true, (approach_length / num_approaches) * i);
            new_leaves[i] = Approach(false, (approach_length / num_approaches) * i);
        };

        // Spawning vehicles
        for (int i = 0; i < num_approaches; i++) {
            auto meta = CellMeta(mNasD, mGen);
            auto vehicle_type = generateType(mGen);
            auto vehicle_length = getTypeLength(vehicle_type);
            auto free_space = approaches[i].freeSpace(vehicle_length);

            if (meta.mNas > free_space) {
                continue;
            }

            auto shared_meta = std::make_shared<CellMeta>(meta);
            for (int j = 0; j < vehicle_length; j++) {
                approaches[i].mRoad[j] = Cell(vehicle_type, shared_meta);
            }
        }
        // Moving vehicles
        for (int i = 0; i < num_approaches; i++) {
            std::shared_ptr<CellMeta> lastMeta;
            for (int j = 0; j < approach_length; j++) {
                auto& cell = approaches[i].mRoad[j];
                if (cell.mType == CellType::Road) {
                    lastMeta = nullptr;
                    continue;
                }
                auto& meta = cell.mMeta;
                if (meta == lastMeta) {
                    continue;
                }
                lastMeta = meta;

                auto& speed = meta->mSpeed;

                int gap = approaches[i].freeSpace(j);

                if (gap < speed) {
                    speed = gap * (2.0 / 3.0);
                } else {
                    // TODO: Hack
                    speed += 2;
                    if (speed > 14) {
                        speed = 14;
                    }
                    // Hack 2
                    gap = approaches[i].freeSpace(j);
                    if (gap < speed) {
                        speed = gap;
                    }
                }

                if (speed > 0) {
                    std::uniform_real_distribution<> dis(0, 1);
                    float r = dis(mGen);
                    if (r < pb) {
                        speed -= 1;
                    }
                }

                // If we are at the end of the approach
                // if (approaches[i].distanceFromEnd(j) == 0) {
                //     auto [space_behind, space_ahead] =
                //     mRoundabout.getSpace(approaches[i].mIndex);

                //     // Error in paper
                //     if (space_behind >= meta->mNas && getTypeLength(cell.mType) <= space_ahead) {
                //         for (int k = j; k < approach_length && approaches[i].mRoad[k].mMeta ==
                //         meta;
                //              k++) {
                //             mRoundabout.mRoad[approaches[i].mIndex + k - j] =
                //                 approaches[i].mRoad[k];
                //         }
                //     }
                //     break;
                // }

                // std::cout << "Distance from end: " << approaches[i].distanceFromEnd(j) <<
                // std::endl;
                if (speed > approaches[i].distanceFromEnd(j)) {
                    speed = approaches[i].distanceFromEnd(j);
                }

                // Update car
                for (int k = j; k < approach_length && approaches[i].mRoad[k].mMeta == meta; k++) {
                    new_approaches[i].mRoad[k + speed] = approaches[i].mRoad[k];
                }
            }
        }

        // Todo update cars on roundabout
        for (int i = 0; i < approach_length; i++) {
            new_roundabout_road[i] = mRoundabout.mRoad[i];
        }

        // Copy results
        for (int i = 0; i < num_approaches; i++) {
            approaches[i] = new_approaches[i];
            leaves[i] = new_leaves[i];
        };

        mRoundabout.mRoad = new_roundabout_road;
    };

    void print() {
        for (auto& a : approaches) {
            for (auto& c : a.mRoad) {
                printCellType(c.mType);
            }
            std::cout << " " << a.mIndex;
            std::cout << std::endl << std::endl;
        }
        std::cout << "---" << std::endl;
        for (auto& c : mRoundabout.mRoad) {
            printCellType(c.mType);
        }
        std::cout << std::endl;
    }

    void run(int num_iters) {
        for (int i = 0; i < num_iters; i++) {
            update();
            print();
            std::cout << "===========================================" << std::endl;
        }
    }
};

int main() {
    Simulation sim;
    sim.run(30);
    return 0;
}
