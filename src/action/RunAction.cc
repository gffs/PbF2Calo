#include <cstring>
#include <string>
#include <mutex>

#include "RunAction.h"
#include "MuonTrackSniffer.h"

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

#include "iostream"

std::mutex root_extra_mutex;

RunAction::RunAction(const json11::Json& cfg):
    G4UserRunAction(),
    cfg_(cfg),
    product_run_map({}),
    root_file(0)
{
    //register this thread with root
    TThread();
}


RunAction::~RunAction()
{
}

const ProductBase& RunAction::operator[](const std::string& key)
{
    return const_cast<const ProductBase&>(
        *(product_run_map[key])
    );
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    //ROOT thread safety is special
    std::unique_lock<std::mutex> root_extra_lock(root_extra_mutex);

    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    std::string root_file_name("calo_");
    root_file_name += std::to_string(G4Threading::G4GetThreadId());
    root_file_name += ".root";

    root_file = new TFile(root_file_name.c_str(), "recreate");


    //auto products = cfg_["products"];
    if (cfg_.is_null()) return;

    for (auto& prd: cfg_.object_items()) {
        product_run_map.emplace(prd.first,
            ProductBase::init(prd.first, prd.second.string_value(), *root_file)
        );
    }
}

void RunAction::EndOfRunAction(const G4Run*)
{
    //ROOT thread safety is unique
    std::unique_lock<std::mutex> root_extra_lock(root_extra_mutex);

    if (root_file) {
        root_file->cd();
        root_file->Write();
        root_file->Close();

        delete root_file;
    }

    for (auto& prd: product_run_map) {
        delete prd.second;
    }

    product_run_map.clear();
}

void RunAction::FillRootTree() const
{
    std::unique_lock<std::mutex> root_extra_lock(root_extra_mutex);

    for (auto& prd: product_run_map) {
        prd.second->FillTree();
    }
}
