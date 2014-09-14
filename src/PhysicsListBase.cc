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
    G4VModularPhysicsList()
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
    G4ProductionCutsTable* pct = G4ProductionCutsTable::GetProductionCutsTable();
    pct->SetEnergyRange(100*eV, pct->GetHighEdgeEnergy());

    auto cuts = cfg_["production_cuts"];
    if (cuts.is_null()) { return; }

    G4RegionStore* rs = G4RegionStore::GetInstance();

    for (auto& p: cuts.object_items()) {
        G4Region* r = nullptr;
        r = rs->GetRegion(p.first);
        if (!r) { continue; }

        auto vec = p.second.array_items();
        std::vector<double> g4cuts;
        std::transform(vec.begin(), vec.end(), std::back_inserter(g4cuts),
                [](const json11::Json& v) { return v.number_value() * um; });

        G4ProductionCuts* pc = new G4ProductionCuts();
        pc->SetProductionCuts(g4cuts);
        r->SetProductionCuts(pc);
    }


}

