#pragma once

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "json11.hpp"

class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(const json11::Json cfg);
    EventAction(): EventAction(json11::Json()) {}
    ~EventAction() {}

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
               
  private:
    json11::Json cfg_;
    G4int fPrintModulo;
    const RunAction* ra;
};

