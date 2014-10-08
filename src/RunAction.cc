#include <cstring>
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

struct energy_deposit {
    float eng;
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    unsigned int ev_num;
    char vol_nm[8];
    char par_nm[8];
};

struct photon_deposit {
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    float mom_x;
    float mom_y;
    float mom_z;
    float kin;
    unsigned int ev_num;
    char vol_nm[8];
    char par_nm[8];
};

struct secondaries_deposit {
    float kin;
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    unsigned int ev_num;
    char vol_nm[8];
    char par_nm[8];
    char proc_nm[8];
};

struct photon_det_deposit {
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    float mom_x;
    float mom_y;
    float mom_z;
    unsigned int ev_num;
};

RunAction::RunAction() : 
    G4UserRunAction(),
    root_file(0),
    eng_q(new std::queue<struct energy_deposit>()),
    eng_tree(0),
    eng_dep(0),
    sec_q(new std::queue<struct secondaries_deposit>()),
    sec_tree(0),
    sec_dep(0),
    pht_q(new std::queue<struct photon_deposit>()),
    pht_tree(0),
    pht_dep(0),
    pht_det_q(new std::queue<struct photon_det_deposit>()),
    pht_det_tree(0),
    pht_det_dep(0)
{
    //register this thread with root
    TThread();
}


RunAction::~RunAction()
{
    delete eng_q;
    delete sec_q;
    delete pht_q;
    delete pht_det_q;
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
    sec_dep = new secondaries_deposit();
    pht_dep = new photon_deposit();
    pht_det_dep = new photon_det_deposit();
    root_file = new TFile(root_file_name.c_str(), "recreate");

    eng_tree = new TTree("eng", "eng");
    eng_tree->Branch("eng_dep", &eng_dep->eng,
            "eng/F:pos_x:pos_y:pos_z:time_g:ev_num/i:vol_nm[8]/C:par_nm[8]/C", 1024*1024);
    eng_tree->Fill();
    eng_tree->Reset();

    sec_tree = new TTree("sec", "sec");
    sec_tree->Branch("sec_dep", &sec_dep->kin,
            "kin/F:pos_x:pos_y:pos_z:time_g:ev_num/i:vol_nm[8]/C:par_nm[8]/C:proc_nm[8]/C", 1024*1024);
    sec_tree->Fill();
    sec_tree->Reset();

    pht_tree = new TTree("pht_org", "pht_org");
    pht_tree->Branch("pht_dep", &pht_dep->pos_x, "pos_x/F:pos_y:pos_z:time_g:mom_x:mom_y:mom_z:kin:ev_num/i:vol_nm[8]/C:par_nm[8]/C", 1024*1024);
    pht_tree->Fill();
    pht_tree->Reset();

    pht_det_tree = new TTree("pht_det", "pht_det");
    pht_det_tree->Branch("pht_det_dep", &pht_det_dep->pos_x, "pos_x/F:pos_y:pos_z:time_g:mom_x:mom_y:mom_z:ev_num/i", 1024*1024);
    pht_det_tree->Fill();
    pht_det_tree->Reset();
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
    delete sec_dep;
    delete pht_dep;
    delete pht_det_dep;
}

void RunAction::FillEnergyDeposit(G4double eng, const G4ThreeVector& pos,
        G4double gtime, G4int evID, const G4String& physVol,
        const G4String& particleName) const
{
    struct energy_deposit ed{
        static_cast<float>(eng),
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<unsigned int>(evID),
        {}, {}
    };

    strncpy(ed.vol_nm, physVol.c_str(), 7);
    ed.vol_nm[7] = '\0';
    strncpy(ed.par_nm, particleName.c_str(), 7);
    ed.par_nm[7] = '\0';

    eng_q->push(ed);
}

void RunAction::FillSecondariesDeposit(G4double eng, const G4ThreeVector& pos,
        G4double gtime, G4int evID, const G4String& physVol,
        const G4String& particleName, const G4String& procName) const
{
    struct secondaries_deposit sd{
        static_cast<float>(eng),
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<unsigned int>(evID),
        {}, {}, {}
    };

    strncpy(sd.vol_nm, physVol.c_str(), 7);
    sd.vol_nm[7] = '\0';
    strncpy(sd.par_nm, particleName.c_str(), 7);
    sd.par_nm[7] = '\0';
    strncpy(sd.proc_nm, procName.c_str(), 7);
    sd.par_nm[7] = '\0';

    sec_q->push(sd);
}

void RunAction::FillPhotonDeposit(const G4ThreeVector& pos, G4double gtime,
        const G4ThreeVector& mom, G4int evID, G4double kin,
        const G4String& physVol, const G4String& particleName) const
{
    struct photon_deposit pd = {
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<float>(mom.x()),
        static_cast<float>(mom.y()),
        static_cast<float>(mom.z()),
        static_cast<float>(kin),
        static_cast<unsigned int>(evID),
        {}, {}
    };

    strncpy(pd.vol_nm, physVol.c_str(), 7);
    pd.vol_nm[7] = '\0';
    strncpy(pd.par_nm, particleName.c_str(), 7);
    pd.par_nm[7] = '\0';

    pht_q->push(pd);
}

void RunAction::FillPhotonDetDeposit(const G4ThreeVector& pos, G4double gtime,
        const G4ThreeVector& mom, G4int evID) const
{
    struct photon_det_deposit pdd = {
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<float>(mom.x()),
        static_cast<float>(mom.y()),
        static_cast<float>(mom.z()),
        static_cast<unsigned int>(evID),
    };

    pht_det_q->push(pdd);
}

void RunAction::FillRootTree() const
{
    std::unique_lock<std::mutex> root_extra_lock(root_extra_mutex);

    while (!eng_q->empty()) {
        *eng_dep = eng_q->front();
        eng_q->pop();
        eng_tree->Fill();
    }

    while (!sec_q->empty()) {
        *sec_dep = sec_q->front();
        sec_q->pop();
        sec_tree->Fill();
    }

    while (!pht_q->empty()) {
        *pht_dep = pht_q->front();
        pht_q->pop();
        pht_tree->Fill();
    }

    while (!pht_det_q->empty()) {
        *pht_det_dep = pht_det_q->front();
        pht_det_q->pop();
        pht_det_tree->Fill();
    }    
}

