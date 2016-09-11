#include "G4EventManager.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4UserRunAction.hh"
#include "RunAction.h"
#include "MuonTrackSniffer.h"
#include "MuonTrackSnifferModel.h"

#include <mutex>

std::mutex MuonTrackSniffer_mutex;

MuonTrackSnifferModel::MuonTrackSnifferModel(const G4String& name,
        G4Region* const regionSniff):
    G4VFastSimulationModel(name, regionSniff)
{
    const G4UserRunAction* const ura =
        G4RunManager::GetRunManager()->GetUserRunAction();
    ra_ = const_cast<RunAction*>(dynamic_cast<const RunAction*>(ura));
}

G4bool MuonTrackSnifferModel::IsApplicable(const G4ParticleDefinition& pd)
{
    if (&pd == G4MuonPlus::Definition() || &pd == G4MuonMinus::Definition()) {
        return true;
    }
    return false;
}

G4bool MuonTrackSnifferModel::ModelTrigger(const G4FastTrack& aFastTrack)
{
    std::unique_lock<std::mutex> MuonTrackSniffer_lock(MuonTrackSniffer_mutex);

    const MuonTrackSniffer& mts = dynamic_cast<const MuonTrackSniffer&>((*ra_)["MuonTrackSniffer"]);

    const G4Track& t = *aFastTrack.GetPrimaryTrack();
    const G4Event& e = *(G4EventManager::GetEventManager()->GetConstCurrentEvent());

    mts.Fill(t, e);

    return false;
}

void MuonTrackSnifferModel::DoIt(const G4FastTrack&, G4FastStep&)
{
    //std::unique_lock<std::mutex> MuonTrackSniffer_lock(MuonTrackSniffer_mutex);
}
