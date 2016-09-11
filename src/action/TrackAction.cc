#include "G4Electron.hh"
#include "G4OpticalPhoton.hh"
#include "G4Positron.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4UserRunAction.hh"
#include "PhotonTrackInformation.h"
#include "RunAction.h"
#include "TrackAction.h"

TrackAction::TrackAction():
    G4UserTrackingAction(),
    G4UserStackingAction()
{
    const G4UserRunAction* const ura =
        G4RunManager::GetRunManager()->GetUserRunAction();
    ra = dynamic_cast<const RunAction*>(ura);
}

TrackAction::~TrackAction()
{
    G4RunManager* rm = G4RunManager::GetRunManager();
    rm->SetUserAction(static_cast<G4UserStackingAction*>(nullptr));
    rm->SetUserAction(static_cast<G4UserTrackingAction*>(nullptr));
}

void TrackAction::PreUserTrackingAction(const G4Track* aTrack)
{
    if (aTrack->GetParticleDefinition() == G4OpticalPhoton::Definition()) {
        fpTrackingManager->SetUserTrackInformation(
            new PhotonTrackInformation(
                aTrack->GetPosition(), aTrack->GetMomentum())
        );
    }
}

G4ClassificationOfNewTrack TrackAction::ClassifyNewTrack(const G4Track* /* aTrack */)
{

    G4ClassificationOfNewTrack classification = fUrgent;
    //if(aTrack->GetParentID() != 0) classification = fKill;

    return classification;
}
