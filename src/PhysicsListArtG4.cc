#include <CLHEP/Units/SystemOfUnits.h>
#include "G4DataQuestionaire.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4ProcessManager.hh"
#include "G4StoppingPhysics.hh"
#include "OpticalPhysics.h"
#include "PhysicsListArtG4.h"

PhysicsListArtG4::PhysicsListArtG4() :
    PhysicsListBase(),
    emPhysicsList(new G4EmStandardPhysics),
    emExtraPhysicsList(new G4EmExtraPhysics),
    decayPhysicsList(new G4DecayPhysics),
    hadronElasticPhysicsList(new G4HadronElasticPhysics),
    hadronPhysicsList(new G4HadronPhysicsFTFP_BERT),
    stoppingPhysicsList(new G4StoppingPhysics),
    ionPhysicsList(new G4IonPhysics),
    opticalPhysicsList(new OpticalPhysics)
{
    G4DataQuestionaire it(photon);
    SetDefaultCutValue(0.7*CLHEP::mm);  
    SetVerboseLevel(0);

    //this->RegisterPhysics(new G4NeutronTrackingCut(0));
}


PhysicsListArtG4::~PhysicsListArtG4()
{
    delete emPhysicsList;
    delete emExtraPhysicsList;
    delete decayPhysicsList;
    delete hadronElasticPhysicsList;
    delete hadronPhysicsList;
    delete stoppingPhysicsList;
    delete ionPhysicsList;
    delete opticalPhysicsList;
}


void PhysicsListArtG4::ConstructParticle()
{
    emPhysicsList->ConstructParticle();
    decayPhysicsList->ConstructParticle();
    opticalPhysicsList->ConstructParticle();
}


void PhysicsListArtG4::ConstructProcess()
{
    AddTransportation();
    AddParametrisation();
    emPhysicsList->ConstructProcess();
    decayPhysicsList->ConstructProcess();
    hadronElasticPhysicsList->ConstructProcess();
    hadronPhysicsList->ConstructProcess();
    stoppingPhysicsList->ConstructProcess();
    ionPhysicsList->ConstructProcess();
    opticalPhysicsList->ConstructProcess();
}

