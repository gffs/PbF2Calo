#include "EventAction.h"
#include "RunAction.h"

#include "EventActionMessenger.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

EventAction::EventAction() :
    G4UserEventAction(),
    fPrintModulo(10),
    fEventMessenger(new EventActionMessenger(this))
{
        const G4UserRunAction* const ura = G4RunManager::GetRunManager()->GetUserRunAction();
        ra = dynamic_cast<const RunAction*>(ura);
}

EventAction::~EventAction()
{
    delete fEventMessenger;
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    G4int evtNb = evt->GetEventID();
 
    //printing survey
    if (evtNb%fPrintModulo == 0) 
    G4cout << "\n---> Begin of Event: " << evtNb << G4endl;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    ra->FillRootTree();
}

