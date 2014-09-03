#include "AlPlateModel.h"
#include "G4FastSimulationManagerProcess.hh"
#include "G4GlobalFastSimulationManager.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4VFastSimulationModel.hh"
#include "SiPlateModel.h"

#include "PhysicsListBase.h"
#include "PhysicsList.h"
#include "PhysicsListArtG4.h"


std::unordered_map<std::string, PhysicsListBase*(*)()>
PhysicsListBase::list_map = {
    {"uw", &PhysicsListBase::create<PhysicsList>},
    {"artg4", &PhysicsListBase::create<PhysicsListArtG4>}
};


PhysicsListBase::PhysicsListBase():
    G4VModularPhysicsList(),
    fCutForGamma(100 * um),
    fCutForElectron(20 * um),
    fCutForPositron(20 * um)
{
    G4LossTableManager::Instance();
}


PhysicsListBase* PhysicsListBase::init(const json11::Json& cfg)
{
    cfg_ = cfg;
    std::string m_key = "uw";
    auto name = cfg["name"];

    if (name.is_null()) {
        std::cout << "\"physics_list\" key missing in json config file.\n"
            << "initializing default \"uw\" physics list." << std::endl;
    }
    else { m_key = name.string_value(); }

    auto it = list_map.find(m_key);
    if (it == list_map.end()) { return nullptr; }
    return it->second();
}


json11::Json PhysicsListBase::cfg_ = json11::Json();


void PhysicsListBase::AddParametrisation()
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


void PhysicsListBase::SetCuts()
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

