#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4UserRunAction.hh"
#include "RunAction.h"
#include "SiPlateModel.h"

#include <mutex>

std::mutex SiPlate_mutex;

SiPlateModel::SiPlateModel(const G4String& name,
        G4Region* const regionSiPlate):
    G4VFastSimulationModel(name, regionSiPlate)
{
    const G4UserRunAction* const ura =
        G4RunManager::GetRunManager()->GetUserRunAction();
    ra_ = dynamic_cast<const RunAction*>(ura);
}

G4bool SiPlateModel::IsApplicable(const G4ParticleDefinition&)
{
    return true;
}

G4bool SiPlateModel::ModelTrigger(const G4FastTrack&)
{
    return true;
}

void SiPlateModel::DoIt(const G4FastTrack& aTrack, G4FastStep& aStep)
{
    std::unique_lock<std::mutex> SiPlate_lock(SiPlate_mutex);

    aStep.KillPrimaryTrack();
    aStep.ProposePrimaryTrackPathLength(0.0);
    aStep.ProposeTotalEnergyDeposited(
            aTrack.GetPrimaryTrack()->GetKineticEnergy());

    //todo: a proper sipm response
}

