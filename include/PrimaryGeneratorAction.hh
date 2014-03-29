#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;

class PrimaryGeneratorAction :
    public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);
    G4ParticleGun* GetParticleGun() {return fParticleGun;};

  private:
    G4ParticleGun* fParticleGun;
};

