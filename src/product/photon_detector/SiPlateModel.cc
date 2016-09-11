#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4UserRunAction.hh"
#include "PhotonDetector.h"
#include "PhotonTrackInformation.h"
#include "RunAction.h"
#include "SiPlateModel.h"

#include <mutex>

std::mutex SiPlate_mutex;

SiPlateModel::SiPlateModel(const G4String& name,
        G4Region* const regionSiPlate):
    G4VFastSimulationModel(name, regionSiPlate)
{
    const G4UserRunAction* const ura = G4RunManager::GetRunManager()->GetUserRunAction();
    ra = const_cast<RunAction*>(dynamic_cast<const RunAction*>(ura));
}

G4bool SiPlateModel::IsApplicable(const G4ParticleDefinition&)
{
    return true;
}

G4bool SiPlateModel::ModelTrigger(const G4FastTrack& aFastTrack)
{
    const G4Track* t = aFastTrack.GetPrimaryTrack();
    if (t->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        if (t->GetVolume()->GetName() == "CalorimeterSiPMActive" &&
                t->GetStep()->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
            return true;
        }
        else { return false; }
    }

    return true;
}

void SiPlateModel::DoIt(const G4FastTrack& aTrack, G4FastStep& aStep)
{
    std::unique_lock<std::mutex> SiPlate_lock(SiPlate_mutex);

    aStep.KillPrimaryTrack();
    aStep.ProposePrimaryTrackPathLength(0.0);
    aStep.ProposeTotalEnergyDeposited(
            aTrack.GetPrimaryTrack()->GetKineticEnergy());

    const G4Track* tr = aTrack.GetPrimaryTrack();

    if (tr->GetParticleDefinition() !=
            G4OpticalPhoton::OpticalPhotonDefinition()) {
        return;
    }
    PhotonTrackInformation* pti =
        static_cast<PhotonTrackInformation*>(tr->GetUserInformation());

    const G4ThreeVector pos = tr->GetPosition();
    const G4ThreeVector mom = tr->GetMomentum();
    G4double gtime = tr->GetGlobalTime();
    G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    const G4ThreeVector pos_org = pti->pos_org;

    //check for leakage in the SiPM sandwich
    //to be replaced by a proper SiPM model
    bool isOK = true;
    for (auto& i: {1, 2}) {
        auto idx = std::floor((pos[i] - 12.5) / 25);
        auto idx_org = std::floor((pos_org[i] - 12.5) / 25);
        if (isOK && idx != idx_org) {isOK = false; }
    }

    //if (!isOK) { return; }

    const PhotonDetector& pd = dynamic_cast<const PhotonDetector&>((*ra)["PhotonDetector"]);
    pd.Fill(pos, gtime, mom, evID, pos_org, pti->mom_org, pti->num_bounces);
}
