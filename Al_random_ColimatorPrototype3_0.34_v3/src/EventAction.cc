#include "EventAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include <iostream>

namespace B1 {

EventAction::EventAction() : G4UserEventAction(), fEnergyThreshold(0.0), fEventCount(0) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    fHitPositions.clear();
    fThetas.clear();
    fPhis.clear();
    fEnergies.clear();
    fParticleTypes.clear();
}

void EventAction::EndOfEventAction(const G4Event*) {
    fEventCount++;
    if (fEventCount % 100000 == 0) {
        G4cout << "EventAction: " << fEventCount << " events processed." << G4endl;
    }

    if (fHitPositions.empty()) {
        return;
    }

    std::ofstream outFile("hits.txt", std::ios::app);
    if (!outFile.is_open()) {
        G4cerr << "Error: Cannot open hits.txt for writing." << G4endl;
        return;
    }

    for (size_t i = 0; i < fHitPositions.size(); ++i) {
        outFile << "θ=" << fThetas[i] / CLHEP::degree << " "
                << "φ=" << fPhis[i] / CLHEP::degree << " "
                << "X=" << fHitPositions[i].x() / CLHEP::cm << " "
                << "Y=" << fHitPositions[i].y() / CLHEP::cm << " "
                << "Z=" << fHitPositions[i].z() / CLHEP::cm << " "
                << "Particle=" << fParticleTypes[i] << "\n";
    }
    outFile.close();
}

void EventAction::RecordHit(const G4ThreeVector& position, G4double theta, G4double phi, G4double energy, const G4String& particleType) {
    if (energy < fEnergyThreshold) {
        return;
    }
    fHitPositions.push_back(position);
    fThetas.push_back(theta);
    fPhis.push_back(phi);
    fEnergies.push_back(energy);
    fParticleTypes.push_back(particleType);
}

}
