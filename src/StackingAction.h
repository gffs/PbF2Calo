#pragma once

#include "G4UserStackingAction.hh"
#include "globals.hh"

class G4Track;
class RunAction;

class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction();
    ~StackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

  private:
    const RunAction* ra;
};

