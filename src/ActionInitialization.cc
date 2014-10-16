#include "ActionInitialization.h"
#include "EventAction.h"
#include "PrimaryGeneratorAction.h"
#include "RunAction.h"
#include "SteppingAction.h"
#include "TrackAction.h"

ActionInitialization::ActionInitialization(const json11::Json& cfg): 
    G4VUserActionInitialization(),
    cfg_(cfg)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    //SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction(cfg_["generator"]));
    SetUserAction(new RunAction);
    SetUserAction(new EventAction(cfg_["event"]));
    TrackAction* ta = new TrackAction;
    SetUserAction(static_cast<G4UserStackingAction*>(ta));
    SetUserAction(static_cast<G4UserTrackingAction*>(ta));
    SetUserAction(new SteppingAction);
}

