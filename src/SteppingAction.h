#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class RunAction;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    const RunAction* ra;
};

