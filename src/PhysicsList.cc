#include "G4DecayPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4ProcessManager.hh"
#include "G4String.hh"
#include "OpticalPhysics.h"
#include "PhysicsList.h"

PhysicsList::PhysicsList() :
    PhysicsListBase(),
    emPhysicsList(new G4EmPenelopePhysics()),
    emExtraPhysicsList(new G4EmExtraPhysics()),
    decayPhysicsList(new G4DecayPhysics()),
    hadronPhysicsList(new G4HadronPhysicsQGSP_BIC_HP()),
    opticalPhysicsList(new OpticalPhysics())
{
    G4String stateOn("on");
    G4String stateOff("off");
    emExtraPhysicsList->Synch(stateOff);
    emExtraPhysicsList->GammaNuclear(stateOn);
    emExtraPhysicsList->MuonNuclear(stateOn);

    SetVerboseLevel(0);
}


PhysicsList::~PhysicsList()
{
    delete decayPhysicsList;
    delete emPhysicsList;
    delete emExtraPhysicsList;
    delete hadronPhysicsList;
    delete opticalPhysicsList;
}


void PhysicsList::ConstructParticle()
{
    emPhysicsList->ConstructParticle();
    decayPhysicsList->ConstructParticle();
    opticalPhysicsList->ConstructParticle();
}


void PhysicsList::ConstructProcess()
{
    AddTransportation();
    AddParametrisation();
    decayPhysicsList->ConstructProcess();
    emPhysicsList->ConstructProcess();
    emExtraPhysicsList->ConstructProcess();
    hadronPhysicsList->ConstructProcess();
    opticalPhysicsList->ConstructProcess();
}


