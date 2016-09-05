#pragma once

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class OpticalPhysics: public G4VPhysicsConstructor
{                      
  public:
    OpticalPhysics();
    virtual ~OpticalPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();
};

