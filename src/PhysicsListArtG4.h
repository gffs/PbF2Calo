#pragma once

#include "PhysicsListBase.h"

class G4EmExtraPhysics;
class G4DecayPhysics;
class G4HadronPhysicsQGSP_BIC_HP;
class G4HadronPhysicsFTFP_BERT_HP;
class OpticalPhysics;

class PhysicsListArtG4: public PhysicsListBase
{
  public:
    PhysicsListArtG4();
   ~PhysicsListArtG4();

    void ConstructParticle();
    void ConstructProcess();

  private:
    G4VPhysicsConstructor* emPhysicsList;
    G4EmExtraPhysics* emExtraPhysicsList;
    G4DecayPhysics* decayPhysicsList;
    G4HadronPhysicsQGSP_BIC_HP* hadronPhysicsList;
    OpticalPhysics* opticalPhysicsList;
};


