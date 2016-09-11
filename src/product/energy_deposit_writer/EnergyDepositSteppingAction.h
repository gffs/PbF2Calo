#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class RunAction;

class EnergyDepositSteppingAction : public G4UserSteppingAction
{
  public:
    EnergyDepositSteppingAction();
    ~EnergyDepositSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    RunAction* ra;
};
