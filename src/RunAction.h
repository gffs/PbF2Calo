#pragma once

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"

#include <queue>

class G4Run;
class TFile;
class TTree;

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

    void FillSecondariesDeposit(G4double eng, const G4ThreeVector& pos,
            G4double gtime, G4int evID, const G4String& physVol,
            const G4String& particleName, const G4String& procName) const;

    void FillPhotonDeposit(const G4ThreeVector& pos, G4double gtime,
            const G4ThreeVector& mom, G4int evID, G4double kin,
            const G4String& physVol, const G4String& particleName,
            const G4String& procName) const;

    void FillPhotonDetDeposit(const G4ThreeVector& pos, G4double gtime,
            const G4ThreeVector& mom, G4int evID, const G4ThreeVector& pos_org,
            const G4ThreeVector& mom_org, const unsigned short num_bounces) const;

    void FillRootTree() const;

  private:
    TFile* root_file;

    std::queue<struct energy_deposit>* eng_q;
    TTree* eng_tree;
    struct energy_deposit* eng_dep;

    std::queue<struct secondaries_deposit>* sec_q;
    TTree* sec_tree;
    struct secondaries_deposit* sec_dep;

    std::queue<struct photon_deposit>* pht_q;
    TTree* pht_tree;
    struct photon_deposit* pht_dep;

    std::queue<struct photon_det_deposit>* pht_det_q;
    TTree* pht_det_tree;
    struct photon_det_deposit* pht_det_dep;
};

