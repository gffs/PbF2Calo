#include "G4DecayPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4ProcessManager.hh"
#include "OpticalPhysics.h"
#include "PhysicsListArtG4.h"

PhysicsListArtG4::PhysicsListArtG4() :
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


PhysicsListArtG4::~PhysicsListArtG4()
{
    delete decayPhysicsList;
    delete emPhysicsList;
    delete emExtraPhysicsList;
    delete hadronPhysicsList;
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
    decayPhysicsList->ConstructProcess();
    emPhysicsList->ConstructProcess();
    emExtraPhysicsList->ConstructProcess();
    hadronPhysicsList->ConstructProcess();
    opticalPhysicsList->ConstructProcess();
}

