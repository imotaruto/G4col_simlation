#ifndef EventAction_hh
#define EventAction_hh

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"

namespace B1 {

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;
    void RecordHit(const G4ThreeVector& position, G4double theta, G4double phi, G4double energy, const G4String& particleType);
    void SetEnergyThreshold(G4double threshold) { fEnergyThreshold = threshold; }

private:
    std::vector<G4ThreeVector> fHitPositions;
    std::vector<G4double> fThetas;
    std::vector<G4double> fPhis;
    std::vector<G4double> fEnergies;
    std::vector<G4String> fParticleTypes;
    G4double fEnergyThreshold;
    G4int fEventCount;
};

}

#endif
