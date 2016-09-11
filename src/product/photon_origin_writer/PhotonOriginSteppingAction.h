#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class RunAction;

class PhotonOriginSteppingAction : public G4UserSteppingAction
{
  public:
    PhotonOriginSteppingAction();
    ~PhotonOriginSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    RunAction* ra;
};
