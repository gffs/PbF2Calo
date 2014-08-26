// modified TestEm17 physics list

#pragma once

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsListMessenger;
class G4VPhysicsConstructor;
class G4EmExtraPhysics;
class G4DecayPhysics;
class G4HadronPhysicsQGSP_BIC_HP;
class G4HadronPhysicsFTFP_BERT_HP;
class OpticalPhysics;


class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList();
   ~PhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();
    void AddPhysicsList(const G4String&);

    virtual void SetCuts();
  private:
    void AddParametrisation();

  private:
    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;
    
    G4VPhysicsConstructor* emPhysicsList;
    G4EmExtraPhysics* emExtraPhysicsList;
    G4DecayPhysics* decayPhysicsList;
    G4HadronPhysicsQGSP_BIC_HP* hadronPhysicsList;
    OpticalPhysics* opticalPhysicsList;
};


