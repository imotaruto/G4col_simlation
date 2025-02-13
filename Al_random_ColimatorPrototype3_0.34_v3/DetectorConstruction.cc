#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4VisAttributes.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "CADMesh.hh"

namespace B1 {

// コンストラクタとデストラクタの定義を修正
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
  // コンストラクタの内容
}

DetectorConstruction::~DetectorConstruction() {
  // デストラクタの内容
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  G4double env_sizeXY = 20*cm, env_sizeZ = 15*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // World
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  auto solidWorld = new G4Box("World", 0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);
  auto logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);

  // Envelope
  auto solidEnv = new G4Box("Envelope", 0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);
  auto logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");
  new G4PVPlacement(nullptr, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  // Additional shape
  G4Material* additional_shape = nist->FindOrBuildMaterial("G4_W");
  double x = 0.0, y = 0.2, z = 0.0;
  auto mesh = CADMesh::TessellatedMesh::FromOBJ("./CollimatorPrototype2_0.34_0.43_v5.obj");
  mesh->SetScale(10.0);
  mesh->SetOffset(G4ThreeVector(x, y, z));
  auto solid_colimator = mesh->GetSolid();
  auto logical_colimator = new G4LogicalVolume(solid_colimator, additional_shape, "colimator");
  new G4PVPlacement(nullptr, G4ThreeVector(), logical_colimator, "colimator", logicEnv, false, 0, checkOverlaps);

  // Pixel Environment and Element setup
  G4double leng_X_PixEnvG = 2.188 *cm, leng_Y_PixEnvG = 2.64 *cm, leng_Z_PixEnvG = 0.03 *cm;
  auto solid_PixEnvG = new G4Box("Solid_PixEnvG", leng_X_PixEnvG/2.0, leng_Y_PixEnvG/2.0, leng_Z_PixEnvG/2.0);
  auto logVol_PixEnvG = new G4LogicalVolume(solid_PixEnvG, env_mat, "LogVol_PixEnvG");
  
  // Create a rotation matrix for XZ-axis 90 degrees rotation
  G4RotationMatrix* rotm = new G4RotationMatrix();
  rotm->rotateX(90.0 * deg);
  rotm->rotateZ(90.0 * deg);
  
  G4ThreeVector pos_LogV_PixEnvG(0.0*cm, 0.0*cm, 0.0*cm);
  new G4PVPlacement(rotm, pos_LogV_PixEnvG, logVol_PixEnvG, "PhysVol_PixEnvG", logicWorld, false, 0, checkOverlaps);

  // Local Envelop setup
  G4int nDiv_Y = 81;
  auto solid_PixEnvL = new G4Box("Solid_PixEnvL", leng_X_PixEnvG/2.0, (leng_Y_PixEnvG/nDiv_Y)/2.0, leng_Z_PixEnvG/2.0);
  auto logVol_PixEnvL = new G4LogicalVolume(solid_PixEnvL, env_mat, "LogVol_PixEnvL");
  new G4PVReplica("PhysVol_PixEnvL", logVol_PixEnvL, logVol_PixEnvG, kYAxis, nDiv_Y, leng_Y_Pix
    EnvG/nDiv_Y);

    // Pixel Element setup
                  G4int nDiv_X = 81;
                  auto solid_PixElmt = new G4Box("Solid_PixElmt", (leng_X_PixEnvG/nDiv_X)/2.0, (leng_Y_PixEnvG/nDiv_Y)/2.0, leng_Z_PixEnvG/2.0);
                  G4Material* materi_PixElmt = nist->FindOrBuildMaterial("G4_Si");
                  auto logVol_PixElmt = new G4LogicalVolume(solid_PixElmt, materi_PixElmt, "LogVol_PixElmt");
                  new G4PVReplica("PhysVol_PixElmt", logVol_PixElmt, logVol_PixEnvL, kXAxis, nDiv_X, leng_X_PixEnvG/nDiv_X);

                  auto aSV = new SensitiveVolume("SensitiveVolume");
                  logVol_PixElmt->SetSensitiveDetector(aSV); // Add sensitivity to the logical volume
                  auto SDman = G4SDManager::GetSDMpointer();
                  SDman->AddNewDetector(aSV);

                  // always return the physical World
                  return physWorld;
                  }

                  } // namespace B1
