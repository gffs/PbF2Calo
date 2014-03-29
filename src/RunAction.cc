#include <string>
#include <mutex>

#include "RunAction.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Threading.hh"

#include "TROOT.h"
#include "TThread.h"
#include "TFile.h"
#include "TTree.h"

RunAction::RunAction() : 
    G4UserRunAction(),
    root_file(0),
    eng_q(new std::queue<struct energy_deposit>()),
    eng_tree(0),
    eng_dep(0),
    pht_q(new std::queue<struct photon_deposit>()),
    pht_tree(0),
    pht_dep(0)
{
    //register this thread with root
    TThread();
}


RunAction::~RunAction()
{
    delete eng_q;
    delete pht_q;
}

std::mutex root_extra_mutex;

void RunAction::BeginOfRunAction(const G4Run*)
{
    //ROOT thread safety is special
    std::unique_lock<std::mutex> root_extra_lock(root_extra_mutex);

    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    std::string root_file_name("calo_hits_");
    root_file_name += std::to_string(G4Threading::G4GetThreadId());
    root_file_name += ".root";

    eng_dep = new energy_deposit();
    pht_dep = new photon_deposit();
    root_file = new TFile(root_file_name.c_str(), "recreate");

    eng_tree = new TTree("eng", "eng");
    eng_tree->Branch("eng_dep", &eng_dep->eng, "eng/F:pos_x:pos_y:pos_z:time_g", 1024*1024);
    eng_tree->Fill();
    eng_tree->Reset();

    pht_tree = new TTree("pht", "pht");
    pht_tree->Branch("pht_dep", &pht_dep->pos_x, "pos_x/F:pos_y:pos_z:time_g:mom_x:mom_y:mom_z", 1024*1024);
    pht_tree->Fill();
    pht_tree->Reset();
}

void RunAction::EndOfRunAction(const G4Run*)
{
    //ROOT thread safety is unique
    std::unique_lock<std::mutex> root_extra_lock(root_extra_mutex);

    root_file->cd();
    root_file->Write();
    root_file->Close();

    delete root_file;
    delete eng_dep;
    delete pht_dep;
}

void RunAction::FillEnergyDeposit(G4double eng, const G4ThreeVector& pos, G4double gtime) const
{
    struct energy_deposit ed{
        static_cast<float>(eng),
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime)
    };

    eng_q->push(ed);
}

void RunAction::FillPhotonDeposit(const G4ThreeVector& pos, G4double gtime, const G4ThreeVector& mom) const
{
    struct photon_deposit pd = {
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<float>(mom.x()),
        static_cast<float>(mom.y()),
        static_cast<float>(mom.z())
    };

    pht_q->push(pd);
}

void RunAction::FillRootTree() const
{
    std::unique_lock<std::mutex> root_extra_lock(root_extra_mutex);

    while (!eng_q->empty()) {
        *eng_dep = eng_q->front();
        eng_q->pop();
        eng_tree->Fill();
    }

    while (!pht_q->empty()) {
        *pht_dep = pht_q->front();
        pht_q->pop();
        pht_tree->Fill();
    }
}


