#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "MyTrackingAction.hh"

namespace B1 {

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction(nullptr));
}

void ActionInitialization::Build() const
{
    auto primaryGenerator = new PrimaryGeneratorAction();
    SetUserAction(primaryGenerator);

    auto runAction = new RunAction(primaryGenerator);
    SetUserAction(runAction);

    auto eventAction = new EventAction();
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(eventAction, primaryGenerator));
    SetUserAction(new MyTrackingAction());  // MyTrackingAction を登録
}

} // namespace B1
