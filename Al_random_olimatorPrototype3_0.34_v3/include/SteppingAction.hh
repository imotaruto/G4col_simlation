#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

namespace B1 {

class EventAction;
class PrimaryGeneratorAction;

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction* eventAction, PrimaryGeneratorAction* primaryGenerator);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*) override;

private:
    EventAction* fEventAction;
    PrimaryGeneratorAction* fPrimaryGenerator;
    G4int fStepCount;
};

}

#endif
