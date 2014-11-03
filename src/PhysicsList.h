#pragma once

#include "PhysicsListBase.h"

class G4EmExtraPhysics;
class G4DecayPhysics;
class G4HadronElasticPhysicsHP;
class G4HadronPhysicsQGSP_BIC_HP;
class G4StoppingPhysics;
class G4IonPhysics;
class OpticalPhysics;

class PhysicsList: public PhysicsListBase
{
  public:
    PhysicsList();
   ~PhysicsList();

    void ConstructParticle();
    void ConstructProcess();
    void AddPhysicsList(const G4String&);

  private:
    G4VPhysicsConstructor* emPhysicsList;
    G4EmExtraPhysics* emExtraPhysicsList;
    G4DecayPhysics* decayPhysicsList;
    G4HadronPhysicsQGSP_BIC_HP* hadronPhysicsList;
    G4HadronElasticPhysicsHP* hadronElasticPhysicsList;
    G4StoppingPhysics* stoppingPhysicsList;
    G4IonPhysics* ionPhysicsList;
    OpticalPhysics* opticalPhysicsList;
};


