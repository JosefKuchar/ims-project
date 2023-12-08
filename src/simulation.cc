#include "simulation.h"
#include <iostream>
#include "approach.h"
#include "settings.h"

Simulation::Simulation() {
    auto& s = Settings::GetInstance();
    mRoundabout = std::make_shared<Roundabout>(Roundabout());
    for (int i = 0; i < s.getApproachCount(); i++) {
        mIncoming.push_back(
            Approach((s.getRoundaboutLength() / s.getApproachCount()) * i + 1, mRoundabout));
    };
}

void Simulation::update() {
    // Spawning vehicles
    int i = 0;
    for (auto& a : mIncoming) {
        a.trySpawnVehicle(i);
        i++;
    }

    // Update vehicles
    for (auto& a : mIncoming) {
        a.update();
    }
    mRoundabout->update();

    // TODO: Update roundabout
    // Update roads
    for (auto& a : mIncoming) {
        a.applyUpdate();
    }
    mRoundabout->applyUpdate();
}

void Simulation::print() {
    std::cout << "Incoming:" << std::endl;
    for (auto& inc : mIncoming) {
        inc.print();
    }

    std::cout << "Outgoing: " << std::endl;
    for (auto& out : mRoundabout->getOutgoing()) {
        out.print();
    }

    std::cout << "Roundabout:" << std::endl;
    mRoundabout->print();
}

void Simulation::run(int num_iters) {
    auto& s = Settings::GetInstance();
    for (int i = 0; i < num_iters; i++) {
        update();
        if (s.getVerbose()) {
            print();
            std::cout << "===========================================" << std::endl;
        }
    }
}
