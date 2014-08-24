#include "SteppingAction.h"
#include "RunAction.h"

#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

static const G4String outOfWorld("OutOfWorld");

SteppingAction::SteppingAction():
    G4UserSteppingAction()
{
        const G4UserRunAction* const ura = G4RunManager::GetRunManager()->GetUserRunAction();
        ra = dynamic_cast<const RunAction*>(ura);
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{  
    G4double edep = aStep->GetTotalEnergyDeposit();

    if (edep > 0) {
        G4StepPoint* endPoint = aStep->GetPostStepPoint();
        const G4ThreeVector pos = endPoint->GetPosition();
        G4double gtime = endPoint->GetGlobalTime();
        auto physVol = endPoint->GetPhysicalVolume();
        G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        if (physVol) {
            ra->FillEnergyDeposit(edep, pos, gtime, evID, physVol->GetName());
        } else {
            ra->FillEnergyDeposit(edep, pos, gtime, evID, outOfWorld);
        }
    }
}

