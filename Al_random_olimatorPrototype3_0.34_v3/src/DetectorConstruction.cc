#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "CADMesh.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RotationMatrix.hh"

namespace B1 {

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    G4double world_sizeXY = 2.0 * 10 * cm;
    G4double world_sizeZ = 2.0 * 10 * cm;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

    G4Box* solidWorld = new G4Box("World", 0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

    G4Material* collimator_mat = nist->FindOrBuildMaterial("G4_W");
    auto mesh = CADMesh::TessellatedMesh::FromOBJ("./ColimatorPrototype3_0.34_v3.obj");
    mesh->SetScale(10.0);
    mesh->SetOffset(G4ThreeVector(0.0, 2.101, 0.0));
    G4VSolid* solidCollimator = mesh->GetSolid();
    G4LogicalVolume* logicCollimator = new G4LogicalVolume(solidCollimator, collimator_mat, "Collimator");
    new G4PVPlacement(nullptr, G4ThreeVector(), logicCollimator, "Collimator", logicWorld, false, 0);

    G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");

    G4double leng_X_PixEnvG = 2.778 * cm;
    G4double leng_Y_PixEnvG = 2.64 * cm;
    G4double leng_Z_PixEnvG = 0.03 * cm;

    G4Material* silicon_mat = nist->FindOrBuildMaterial("G4_W");
    auto solid_SiPlate = new G4Box("Solid_SiPlate", leng_X_PixEnvG / 2.0, leng_Y_PixEnvG / 2.0, leng_Z_PixEnvG / 2.0);
    auto logVol_SiPlate = new G4LogicalVolume(solid_SiPlate, silicon_mat, "LogVol_SiPlate");

    G4RotationMatrix* rotm = new G4RotationMatrix();
    rotm->rotateX(90.0 * deg);
    rotm->rotateZ(90.0 * deg);

    G4ThreeVector pos_SiPlate(0.0 * cm, 0.0 * cm, 0.0 * cm);
    new G4PVPlacement(rotm, pos_SiPlate, logVol_SiPlate, "PhysVol_SiPlate", logicWorld, false, 0, true);

    G4PhysicalVolumeStore* pVolumeStore = G4PhysicalVolumeStore::GetInstance();
    for (size_t i = 0; i < pVolumeStore->size(); ++i) {
        if ((*pVolumeStore)[i]->CheckOverlaps()) {
            G4cout << "Overlap detected for volume: " << (*pVolumeStore)[i]->GetName() << G4endl;
        }
    }

    return physWorld;
}

}
