#include "AlPlateModel.h"
#include "OpticalPhysics.h"
#include "PhysicsList.h"
#include "SiPlateModel.h"

#include "G4DecayPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4LossTableManager.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4ProcessManager.hh"
#include "G4RegionStore.hh"

#include "G4String.hh"
#include "G4SystemOfUnits.hh"

#include "G4GlobalFastSimulationManager.hh"
#include "G4VFastSimulationModel.hh"

PhysicsList::PhysicsList() :
    G4VModularPhysicsList(),
    fCutForGamma(100 * um),
    fCutForElectron(20 * um),
    fCutForPositron(20 * um),
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

    G4LossTableManager::Instance();
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

void PhysicsList::AddParametrisation()
{
    G4RegionStore* rs = G4RegionStore::GetInstance();
    G4Region* aPlate = rs->GetRegion("AlPlate");
    new AlPlateModel("AlPlate", aPlate);
    aPlate = rs->GetRegion("SiPlate");
    new SiPlateModel("SiPlate", aPlate);

    G4FastSimulationManagerProcess* fsmp = new G4FastSimulationManagerProcess();
    theParticleIterator->reset();
    while((*theParticleIterator)()) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        pmanager->AddDiscreteProcess(fsmp);
    }
}

void PhysicsList::AddPhysicsList(const G4String&)
{
}

void PhysicsList::SetCuts()
{
     
  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }  

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");   
    
  if (verboseLevel>0) DumpCutValuesTable();
}

