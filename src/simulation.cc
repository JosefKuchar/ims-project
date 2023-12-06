#include "simulation.h"
#include <iostream>
#include "approach.h"
#include "settings.h"

Simulation::Simulation() {
    auto& s = Settings::GetInstance();
    mRoundabout = Roundabout();
    std::shared_ptr<Roundabout> roundabout = std::make_shared<Roundabout>(mRoundabout);
    for (int i = 0; i < s.getApproachCount(); i++) {
        mIncoming.push_back(
            Approach((s.getApproachLength() / s.getApproachCount()) * i, roundabout));
        mOutgoing.push_back(
            Approach((s.getApproachLength() / s.getApproachCount()) * i, roundabout));
    };
}

void Simulation::update() {
    // Spawning vehicles
    for (auto& a : mIncoming) {
        a.trySpawnVehicle();
    }

    // Update vehicles
    for (auto& a : mIncoming) {
        a.update();
    }

    // TODO: Update roundabout
    // Update roads
    for (auto& a : mIncoming) {
        a.applyUpdate();
    }
    for (auto& a : mOutgoing) {
        a.applyUpdate();
    }
    mRoundabout.applyUpdate();
}

void Simulation::print() {
    std::cout << "Incoming:" << std::endl;
    for (auto& inc : mIncoming) {
        inc.print();
        std::cout << std::endl;
    }
    std::cout << "Outgoing:" << std::endl;
    for (auto& out : mOutgoing) {
        out.print();
        std::cout << std::endl;
    }
    std::cout << "Roundabout:" << std::endl;
    mRoundabout.print();
    std::cout << std::endl;
}

void Simulation::run(int num_iters) {
    for (int i = 0; i < num_iters; i++) {
        update();
        print();
        std::cout << "===========================================" << std::endl;
    }
}
