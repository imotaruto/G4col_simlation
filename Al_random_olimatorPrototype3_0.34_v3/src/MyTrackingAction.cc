#include "MyTrackingAction.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

MyTrackingAction::MyTrackingAction() : G4UserTrackingAction() {}

MyTrackingAction::~MyTrackingAction() {}

void MyTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    // 親粒子以外のトラックをキルする
    if (track->GetParentID() != 0) {
        G4Track* modifiableTrack = const_cast<G4Track*>(track);
        modifiableTrack->SetTrackStatus(fStopAndKill);
 
    }
}
