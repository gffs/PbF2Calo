#pragma once

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "ProductBase.h"
#include "json11.hpp"
#include <queue>

class G4Event;
class G4Track;
class TFile;
class TH3F;

class EnergyDepositWriter: public ProductBase
{
public:
    EnergyDepositWriter(const json11::Json& cfg, TFile& root_file);
    ~EnergyDepositWriter();

    int FillTree() { return 0; } const override;

    int Fill(G4double eng, const G4ThreeVector& pos) const;

private:
    TH3F* energy_deposit_histo;
};
