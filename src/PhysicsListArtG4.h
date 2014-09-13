#pragma once

#include "PhysicsListBase.h"

class G4EmExtraPhysics;
class G4DecayPhysics;
class G4HadronPhysicsFTFP_BERT;
class G4HadronElasticPhysics;
class G4StoppingPhysics;
class G4IonPhysics;
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
    G4HadronElasticPhysics* hadronElasticPhysicsList;
    G4HadronPhysicsFTFP_BERT* hadronPhysicsList;
    G4StoppingPhysics* stoppingPhysicsList;
    G4IonPhysics* ionPhysicsList;
    OpticalPhysics* opticalPhysicsList;
};


