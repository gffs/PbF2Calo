#include "EventAction.h"
#include "RunAction.h"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

EventAction::EventAction(const json11::Json cfg):
    G4UserEventAction(),
    cfg_(cfg),
    fPrintModulo(cfg["printModulo"].number_value())
{
    if (fPrintModulo == 0) { fPrintModulo = 10; }

    const G4UserRunAction* const ura =
        G4RunManager::GetRunManager()->GetUserRunAction();
    ra = dynamic_cast<const RunAction*>(ura);
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    G4int evtNb = evt->GetEventID();
 
    if (evtNb % fPrintModulo == 0) 
    G4cout << "event: " << evtNb << G4endl;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    ra->FillRootTree();
}

