#include "AlPlateModel.h"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"

#include <mutex>

std::mutex AlPlate_mutex;

AlPlateModel::AlPlateModel(const G4String& name,
        G4Region* const regionAlPlate):
    G4VFastSimulationModel(name, regionAlPlate),
    regionAlPlate_(regionAlPlate)
{}

G4bool AlPlateModel::IsApplicable(const G4ParticleDefinition& aParticle)
{
    return (&aParticle != G4OpticalPhoton::OpticalPhotonDefinition());
}

G4bool AlPlateModel::ModelTrigger(const G4FastTrack& aTrack)
{
    std::unique_lock<std::mutex> AlPlate_lock(AlPlate_mutex);

    G4ThreeVector mom(aTrack.GetPrimaryTrackLocalMomentum());
    //ignore particles flying towards the calorimeter
    if (mom.x() > 0) return false;
    return true;
}

void AlPlateModel::DoIt(const G4FastTrack& aTrack, G4FastStep& aStep)
{
    std::unique_lock<std::mutex> AlPlate_lock(AlPlate_mutex);

    aStep.KillPrimaryTrack();
    aStep.ProposePrimaryTrackPathLength(0.0);
    aStep.ProposeTotalEnergyDeposited(
            aTrack.GetPrimaryTrack()->GetKineticEnergy());
}

