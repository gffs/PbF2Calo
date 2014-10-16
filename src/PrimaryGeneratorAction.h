#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "json11.hpp"

class G4Event;
class G4ParticleGun;

class PrimaryGeneratorAction:
    public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(const json11::Json& cfg);
    PrimaryGeneratorAction(): PrimaryGeneratorAction(json11::Json()) {}
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*);
    G4ParticleGun* GetParticleGun() { return fParticleGun; }

  private:
    G4ParticleGun* fParticleGun;
    json11::Json cfg_;
};

