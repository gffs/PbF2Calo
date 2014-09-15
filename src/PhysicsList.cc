#include "G4DataQuestionaire.hh"
#include "G4DecayPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4IonPhysics.hh"
#include "G4LossTableManager.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4ProcessManager.hh"
#include "G4EmProcessOptions.hh"
#include "G4StoppingPhysics.hh"
#include "G4String.hh"
#include "G4VAtomDeexcitation.hh"
#include "OpticalPhysics.h"
#include "PhysicsList.h"

PhysicsList::PhysicsList() :
    PhysicsListBase(),
    emPhysicsList(new G4EmPenelopePhysics),
    emExtraPhysicsList(new G4EmExtraPhysics),
    decayPhysicsList(new G4DecayPhysics),
    hadronElasticPhysicsList(new G4HadronElasticPhysics),
    hadronPhysicsList(new G4HadronPhysicsQGSP_BIC_HP),
    stoppingPhysicsList(new G4StoppingPhysics),
    ionPhysicsList(new G4IonPhysics),
    opticalPhysicsList(new OpticalPhysics)
{
    G4DataQuestionaire it(photon, neutron);

    G4String stateOn("on");
    G4String stateOff("off");
    emExtraPhysicsList->Synch(stateOff);
    emExtraPhysicsList->GammaNuclear(stateOn);
    emExtraPhysicsList->MuonNuclear(stateOn);

    SetVerboseLevel(1);
}


PhysicsList::~PhysicsList()
{
    delete decayPhysicsList;
    delete emPhysicsList;
    delete emExtraPhysicsList;
    delete hadronElasticPhysicsList;
    delete hadronPhysicsList;
    delete stoppingPhysicsList;
    delete ionPhysicsList;
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
    hadronElasticPhysicsList->ConstructProcess();
    hadronPhysicsList->ConstructProcess();
    stoppingPhysicsList->ConstructProcess();
    ionPhysicsList->ConstructProcess();
    opticalPhysicsList->ConstructProcess();

    G4EmProcessOptions opt;
    opt.SetVerbose(1);

    //Penelope EM Physics  
    //opt.SetMscStepLimitation(fUseDistanceToBoundary);
    //opt.SetMscRangeFactor(0.02);
    
    opt.SetMinEnergy(100*eV);
    opt.SetMaxEnergy(10*TeV);
    opt.SetDEDXBinning(320); //default: 220
    opt.SetLambdaBinning(320); //default: 220

    //opt.SetSplineFlag(true); //default: true
    opt.SetPolarAngleLimit(CLHEP::pi);
    
    // Ionization
    //opt.SetSubCutoff(true);    

    opt.SetApplyCuts(true);

    // Deexcitation
    G4VAtomDeexcitation* deex;
    deex = G4LossTableManager::Instance()->AtomDeexcitation();
    deex->SetFluo(false); //penelope default: true
}

