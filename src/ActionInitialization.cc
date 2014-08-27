#include "ActionInitialization.h"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.h"
#include "EventAction.h"
#include "StackingAction.h"
#include "SteppingAction.h"

ActionInitialization::ActionInitialization() : 
    G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    //SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(new RunAction);
    SetUserAction(new EventAction);
    SetUserAction(new StackingAction);
    SetUserAction(new SteppingAction);
}  
