#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <cmath>
#include <CLHEP/Random/RandFlat.h>

namespace B1 {

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
fParticleGun(nullptr),
fTheta(0.0),
fPhi(0.0),
fParticleCount(0) { // カウンターを初期化
    G4int n_particle = 1; // 一度に1000発の粒子を生成
    fParticleGun = new G4ParticleGun(n_particle);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    if (particle) {
        fParticleGun->SetParticleEnergy(10.0 * keV);
        fParticleGun->SetParticleDefinition(particle);
    } else {
        G4cerr << "Error: Particle 'gamma' not found!" << std::endl;
    }
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    G4double radius = 5.0 * cm;
    
    for (int i = 0; i < 1000; ++i) {
        fTheta = std::acos(CLHEP::RandFlat::shoot()) * 0.5 * CLHEP::pi; // 0から90度の範囲でランダムサンプリング
        fPhi = CLHEP::RandFlat::shoot(0., 360.) * CLHEP::degree;        // 0から360度の範囲でランダムサンプリング
        
        G4double x = radius * std::sin(fTheta) * std::cos(fPhi) - 37.57326654* mm;
        G4double y = radius * std::cos(fTheta) + 22.101;
        G4double z = radius * std::sin(fTheta) * std::sin(fPhi) + 7.45* mm;
        
        if (std::abs(z - 7.45 * mm) <= 2.02 * cm && std::abs(x + 37.57326654 * mm) <= 0.4 * cm) {
            fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
            
            G4ThreeVector targetPosition(-13.98 * mm, 22.101 * mm, 7.45 * mm);
            G4ThreeVector momentumDirection = (targetPosition - G4ThreeVector(x, y, z)).unit();
            fParticleGun->SetParticleMomentumDirection(momentumDirection);
            
            fParticleGun->GeneratePrimaryVertex(anEvent);
        }
    }
    
    fParticleCount += 1;
    if (fParticleCount % 10000 == 0) {
        G4cout << "PrimaryGeneratorAction: " << fParticleCount << " particles generated." << G4endl;
    }
}

G4double PrimaryGeneratorAction::GetTheta() const {
    return fTheta;
}
G4double PrimaryGeneratorAction::GetPhi() const {
    return fPhi;
    
}
}
