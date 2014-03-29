#include "SteppingAction.h"
#include "RunAction.h"

#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

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

        ra->FillEnergyDeposit(edep, pos, gtime);
    }
}

