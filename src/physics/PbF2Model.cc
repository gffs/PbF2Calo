#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "G4UserRunAction.hh"
#include "PhotonTrackInformation.h"
#include "PbF2Model.h"

#include <mutex>

std::mutex PbF2_mutex;

PbF2Model::PbF2Model(const G4String& name,
        G4Region* const regionPbF2):
    G4VFastSimulationModel(name, regionPbF2)
{}

G4bool PbF2Model::IsApplicable(const G4ParticleDefinition&)
{
    return true;
}

G4bool PbF2Model::ModelTrigger(const G4FastTrack& aFastTrack)
{
    const G4Track* t = aFastTrack.GetPrimaryTrack();
    if (t->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()
        && t->GetVolume()->GetName() == "CaloCrystalArray"
        && t->GetStep()->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {

        PhotonTrackInformation* pti =
            static_cast<PhotonTrackInformation*>(t->GetUserInformation());
        pti->num_bounces++;
    }

    return false;
}

void PbF2Model::DoIt(const G4FastTrack&, G4FastStep&)
{
    std::unique_lock<std::mutex> PbF2_lock(PbF2_mutex);
}
