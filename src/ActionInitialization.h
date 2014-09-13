#pragma once

#include "G4VUserActionInitialization.hh"
#include "json11.hpp"

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(const json11::Json cfg);
    ActionInitialization(): ActionInitialization(json11::Json()) {}
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    json11::Json cfg_;
};
 
