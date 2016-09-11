#pragma once

#include "EnergyDepositData.h"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "ProductBase.h"
#include "json11.hpp"
#include <queue>

class G4Event;
class G4Track;
class TFile;
class TTree;

class EnergyDepositWriter: public ProductBase
{
public:
    EnergyDepositWriter(const json11::Json& cfg, TFile& root_file);
    ~EnergyDepositWriter();

    virtual int FillTree() const;

    int Fill(G4double eng, const G4ThreeVector& pos,
            G4double gtime, G4int evID, const G4String& physVol,
            const G4String& particleName) const;

    int Fill(const G4Track& t, const G4Event& e) const;

private:
    std::queue<struct energy_deposit_data>* energy_deposit_q;
    TTree* energy_deposit_tree;
    struct energy_deposit_data* energy_deposit;
};
