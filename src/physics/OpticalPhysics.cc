#include "G4OpBoundaryProcess.hh"
#include "G4BuilderType.hh"
#include "G4Cerenkov.hh"
#include "G4OpAbsorption.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ProcessManager.hh"

#include "OpticalPhysics.h"


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

    auto aParticleIterator = GetParticleIterator();
    aParticleIterator->reset();

    while ((*aParticleIterator)()) {
        G4ParticleDefinition* particle = aParticleIterator->value();

        if (cerenkov->IsApplicable(*particle)) {
            ph->RegisterProcess(cerenkov, particle);
        }
    }

    G4ProcessManager* pm = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
    pm->AddDiscreteProcess(new G4OpAbsorption());
    auto opBo = new G4OpBoundaryProcess();
    //opBo->SetVerboseLevel(2);
    pm->AddDiscreteProcess(opBo);
}
