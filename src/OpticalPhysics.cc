#include "G4OpticalPhoton.hh"
#include "G4Cerenkov.hh"

#include "OpticalPhysics.h"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"

OpticalPhysics::OpticalPhysics():
    G4VPhysicsConstructor("OpticalPhysics")
{
    SetPhysicsType(G4BuilderType::bElectromagnetic);
}

OpticalPhysics::~OpticalPhysics()
{
}

void OpticalPhysics::ConstructParticle()
{
    G4OpticalPhoton::OpticalPhotonDefinition();
}

void OpticalPhysics::ConstructProcess()
{
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    G4Cerenkov* cerenkov = new G4Cerenkov();

    aParticleIterator->reset();

    while ((*aParticleIterator)()) {
        G4ParticleDefinition* particle = aParticleIterator->value();

        if (cerenkov->IsApplicable(*particle)) {
            ph->RegisterProcess(cerenkov, particle);
        }
    }
}


