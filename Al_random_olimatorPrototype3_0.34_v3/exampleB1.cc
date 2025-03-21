#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BIC.hh"
#include "PhysicsList.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    G4RunManager* runManager = new G4RunManager;

    runManager->SetUserInitialization(new B1::DetectorConstruction());
    runManager->SetUserInitialization(new eRositaPhysicsList());
    runManager->SetUserInitialization(new B1::ActionInitialization());

    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (!ui) {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
