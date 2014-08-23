#pragma once

#include <queue>

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class G4Run;
class TFile;
class TTree;

struct energy_deposit {
    float eng;
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
};

struct photon_deposit {
    float pos_x;
    float pos_y;
    float pos_z;
    float time_g;
    float mom_x;
    float mom_y;
    float mom_z;
};

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void FillEnergyDeposit(G4double eng, const G4ThreeVector& pos, G4double gtime) const;
    void FillPhotonDeposit(const G4ThreeVector& pos, G4double gtime, const G4ThreeVector& mom) const;
    void FillRootTree() const;

  private:
    TFile* root_file;

    std::queue<struct energy_deposit>* eng_q;
    TTree* eng_tree;
    struct energy_deposit* eng_dep;

    std::queue<struct photon_deposit>* pht_q;
    TTree* pht_tree;
    struct photon_deposit* pht_dep;
};

