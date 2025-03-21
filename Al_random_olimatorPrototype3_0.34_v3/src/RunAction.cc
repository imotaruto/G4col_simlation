#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>

namespace B1 {

RunAction::RunAction(PrimaryGeneratorAction* primary)
 : G4UserRunAction(), fPrimary(primary)
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*) {
    if (IsMaster()) {
        G4cout << "### Run start." << G4endl;
    }

    // Open output file for writing hit data
    std::ofstream outFile("hits.txt", std::ios::trunc);
    if (!outFile.is_open()) {
        G4cerr << "Error: Cannot open hits.txt for writing." << G4endl;
    }
}

void RunAction::EndOfRunAction(const G4Run*) {
    if (IsMaster()) {
        G4cout << "### Run end." << G4endl;
    }
}

} // namespace B1
