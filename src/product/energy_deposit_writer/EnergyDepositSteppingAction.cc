#include "EnergyDepositWriter.h"
#include "EnergyDepositSteppingAction.h"
#include "RunAction.h"

#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

static const G4String outOfWorld("OutOfWorld");

EnergyDepositSteppingAction::EnergyDepositSteppingAction():
    G4UserSteppingAction()
{
        const G4UserRunAction* const ura =
            G4RunManager::GetRunManager()->GetUserRunAction();
        ra = const_cast<RunAction*>(dynamic_cast<const RunAction*>(ura));
}

EnergyDepositSteppingAction::~EnergyDepositSteppingAction()
{
}

void EnergyDepositSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4double edep = aStep->GetTotalEnergyDeposit();
    const EnergyDepositWriter& edw = dynamic_cast<const EnergyDepositWriter&>((*ra)["EnergyDepositWriter"]);

    if (edep > 0 && (aStep->GetTrack()->GetParticleDefinition()
                != G4OpticalPhoton::OpticalPhotonDefinition())) {
        G4StepPoint* endPoint = aStep->GetPostStepPoint();
        const G4ThreeVector pos = endPoint->GetPosition();
        G4double gtime = endPoint->GetGlobalTime();
        auto physVol = endPoint->GetPhysicalVolume();
        G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        const G4String& particleName = aStep->GetTrack()->GetParticleDefinition()->
            GetParticleName();
        if (physVol) {
            edw.Fill(edep, pos, gtime, evID, physVol->GetName(), particleName);
        } else {
            edw.Fill(edep, pos, gtime, evID, outOfWorld, particleName);
        }
    }
}
