#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"

namespace B1 {

class PrimaryGeneratorAction;

class RunAction : public G4UserRunAction {
public:
    RunAction(PrimaryGeneratorAction* primary);
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    PrimaryGeneratorAction* fPrimary;
};

} // namespace B1

#endif // RUNACTION_HH
