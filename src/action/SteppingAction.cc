#include "SteppingAction.h"
#include "RunAction.h"

#include "G4OpticalPhoton.hh"
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
            ra->FillEnergyDeposit(edep, pos, gtime, evID,
                    physVol->GetName(), particleName);
        } else {
            ra->FillEnergyDeposit(edep, pos, gtime, evID,
                    outOfWorld, particleName);
        }
    }

    const std::vector<const G4Track*>* sec = aStep->GetSecondaryInCurrentStep();
    auto proc_cr = aStep->GetTrack()->GetCreatorProcess();
    G4String proc_nm("none");
    if (proc_cr) { proc_nm = proc_cr->GetProcessName(); }

/*
    for (auto aTrack: *sec) {
        if (aTrack->GetParticleDefinition()->GetParticleName() == "opticalphoton") {
            const G4ThreeVector pos = aTrack->GetPosition();
            const G4ThreeVector mom = aTrack->GetMomentum();
            G4double gtime = aTrack->GetGlobalTime();
            G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
            G4double kin = aStep->GetTrack()->GetKineticEnergy();
            auto physVol = aTrack->GetVolume();
            const G4String& particleName = aStep->GetTrack()->
                GetParticleDefinition()->GetParticleName();

            ra->FillPhotonDeposit(pos, gtime, mom, evID, kin, physVol->GetName(),
                    particleName, proc_nm);
        }
    }
*/
}
