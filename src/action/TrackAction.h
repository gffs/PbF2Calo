#pragma once

#include "G4UserTrackingAction.hh"
#include "G4UserStackingAction.hh"

class RunAction;
class G4Track;

class TrackAction: public G4UserTrackingAction, public G4UserStackingAction
{
  public:
    TrackAction();
    ~TrackAction();

    void PreUserTrackingAction(const G4Track* aTrack);
    void PostUserTrackingAction(const G4Track*) {}

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

  private:
    const RunAction* ra;
};

