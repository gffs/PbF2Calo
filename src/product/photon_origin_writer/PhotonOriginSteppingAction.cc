#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"
#include "PhotonOriginWriter.h"
#include "PhotonOriginSteppingAction.h"
#include "RunAction.h"

PhotonOriginSteppingAction::PhotonOriginSteppingAction():
    G4UserSteppingAction()
{
    const G4UserRunAction* const ura = G4RunManager::GetRunManager()->GetUserRunAction();
    ra = const_cast<RunAction*>(dynamic_cast<const RunAction*>(ura));
}

PhotonOriginSteppingAction::~PhotonOriginSteppingAction()
{
}

void PhotonOriginSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    const PhotonOriginWriter& pow = dynamic_cast<const PhotonOriginWriter&>((*ra)["PhotonOriginWriter"]);

    const std::vector<const G4Track*>* sec = aStep->GetSecondaryInCurrentStep();
    auto proc_cr = aStep->GetTrack()->GetCreatorProcess();
    G4String proc_nm("none");
    if (proc_cr) { proc_nm = proc_cr->GetProcessName(); }

    for (auto aTrack: *sec) {
        if (aTrack->GetParticleDefinition()->GetParticleName() == "opticalphoton") {
            const G4ThreeVector pos = aTrack->GetPosition();
            const G4ThreeVector mom = aTrack->GetMomentum();
            G4double gtime = aTrack->GetGlobalTime();
            G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
            G4double kin = aStep->GetTrack()->GetKineticEnergy();
            const G4String& particleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();

            pow.Fill(pos, gtime, mom, evID, kin, particleName, proc_nm);
        }
    }
}
