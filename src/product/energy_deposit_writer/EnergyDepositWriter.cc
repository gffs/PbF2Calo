#include "G4Event.hh"
#include "G4MultiSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"

#include "EnergyDepositSteppingAction.h"
#include "EnergyDepositWriter.h"
#include "TTree.h"
#include "TFile.h"

#include "iostream"

EnergyDepositWriter::EnergyDepositWriter(const json11::Json& cfg, TFile& root_file):
    ProductBase(cfg),
    energy_deposit_q(new std::queue<struct energy_deposit_data>()),
    energy_deposit_tree(0),
    energy_deposit(new energy_deposit_data())
{
    root_file.cd();
    energy_deposit_tree = new TTree("energy_deposit", "energy_deposit");
    energy_deposit_tree->Branch("energy_deposit", &energy_deposit->eng,
            "eng/F:pos_x:pos_y:pos_z:time_g:ev_num/i:vol_nm[16]/C:par_nm[8]/C", 1024*1024);
    energy_deposit_tree->Fill();
    energy_deposit_tree->Reset();


    auto msa = const_cast<G4MultiSteppingAction*>(dynamic_cast<const G4MultiSteppingAction*>(G4RunManager::GetRunManager()->GetUserSteppingAction()));
    msa->push_back(G4UserSteppingActionUPtr(new EnergyDepositSteppingAction));
}


EnergyDepositWriter::~EnergyDepositWriter()
{
    delete energy_deposit;
    delete energy_deposit_q;
}


int EnergyDepositWriter::FillTree() const
{
    while (!energy_deposit_q->empty()) {
        *energy_deposit = energy_deposit_q->front();
        energy_deposit_q->pop();
        energy_deposit_tree->Fill();
    }

    return 0;
}


int EnergyDepositWriter::Fill(G4double eng, const G4ThreeVector& pos,
        G4double gtime, G4int evID, const G4String& physVol,
        const G4String& particleName) const
{
    struct energy_deposit_data ed{
        static_cast<float>(eng),
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<unsigned int>(evID),
        {}, {}
    };

    strncpy(ed.vol_nm, physVol.c_str(), 15);
    ed.vol_nm[15] = '\0';
    strncpy(ed.par_nm, particleName.c_str(), 7);
    ed.par_nm[7] = '\0';

    energy_deposit_q->push(ed);

    return 0;
}
