#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "PhotonTrackInformation.h"
#include "TrackingAction.h"

TrackingAction::TrackingAction(): 
    G4UserTrackingAction()
{}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    if (aTrack->GetParticleDefinition() 
            == G4OpticalPhoton::OpticalPhotonDefinition()) {

        fpTrackingManager->SetUserTrackInformation(new PhotonTrackInformation);
    }
}

