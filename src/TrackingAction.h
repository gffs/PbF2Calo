#pragma once

#include "G4UserTrackingAction.hh"

class G4Track;

class TrackingAction : public G4UserTrackingAction
{
  public:
    TrackingAction();
    ~TrackingAction() {}

    void PreUserTrackingAction(const G4Track* aTrack);
    void PostUserTrackingAction(const G4Track*) {}
};

