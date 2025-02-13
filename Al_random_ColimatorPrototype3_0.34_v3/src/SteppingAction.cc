#include "SteppingAction.hh"
#include "EventAction.hh"  // EventAction の完全な定義を得るためにインクルード
#include "PrimaryGeneratorAction.hh"  // PrimaryGeneratorAction の完全な定義を得るためにインクルード
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

namespace B1 {

SteppingAction::SteppingAction(EventAction* eventAction, PrimaryGeneratorAction* primaryGenerator)
: G4UserSteppingAction(), fEventAction(eventAction), fPrimaryGenerator(primaryGenerator) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    auto track = step->GetTrack();
    auto postStepPoint = step->GetPostStepPoint();
    auto volume = postStepPoint->GetTouchableHandle()->GetVolume();




    
    // Check if volume is valid and if the particle is in PhysVol_SiPlate
    if (volume && volume->GetName() == "PhysVol_SiPlate") {
        auto position = postStepPoint->GetPosition();
        auto energy = postStepPoint->GetKineticEnergy();
        auto particleType = track->GetDefinition()->GetParticleName();

        // Record the hit in EventAction
        fEventAction->RecordHit(position, fPrimaryGenerator->GetTheta(), fPrimaryGenerator->GetPhi(), energy, particleType);

        // Kill the track to prevent further processing
        track->SetTrackStatus(fStopAndKill);
    }
}

}
