#pragma once

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"

#include <queue>

class G4Run;
class TFile;
class TTree;

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
};

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

    void FillEnergyDeposit(G4double eng, const G4ThreeVector& pos,
            G4double gtime, G4int evID, const G4String& physVol,
            const G4String& particleName) const;
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

