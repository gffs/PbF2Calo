#pragma once

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventActionMessenger;
class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
    void SetPrintModulo(G4int val) {fPrintModulo = val;};
               
  private:
    G4int                 fPrintModulo;                    
    EventActionMessenger* fEventMessenger;
    const RunAction* ra;
};

