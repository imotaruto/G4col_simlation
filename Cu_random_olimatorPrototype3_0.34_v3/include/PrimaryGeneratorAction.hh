#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

namespace B1 {

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);

    void SetParticleMomentumDirection(G4double theta, G4double phi);

    G4double GetTheta() const;
    G4double GetPhi() const;

private:
    G4ParticleGun* fParticleGun;
    G4double fTheta;
    G4double fPhi;
    G4int fParticleCount;
};

}

#endif
