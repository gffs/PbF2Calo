#include "PhysicsList.h"
#include "OpticalPhysics.h"

#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4LossTableManager.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4String.hh"
#include "G4SystemOfUnits.hh"

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
    emExtraPhysicsList->MuonNuclear(stateOff);

    SetVerboseLevel(1);

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
    decayPhysicsList->ConstructProcess();
    emPhysicsList->ConstructProcess();
    emExtraPhysicsList->ConstructProcess();
    hadronPhysicsList->ConstructProcess();
    opticalPhysicsList->ConstructProcess();
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

void PhysicsList::SetCutForGamma(G4double cut)
{
  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForElectron(G4double cut)
{
  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForPositron(G4double cut)
{
  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

