#pragma once

#include "PhotonOriginData.h"
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

class PhotonOriginWriter: public ProductBase
{
public:
    PhotonOriginWriter(const json11::Json& cfg, TFile& root_file);
    ~PhotonOriginWriter();

    virtual int FillTree() const;

    int Fill(const G4ThreeVector& pos, G4double gtime,
            const G4ThreeVector& mom, G4int evID, G4double kin,
            const G4String& particleName, const G4String& procName) const;

private:
    std::queue<struct photon_origin_data>* photon_origin_q;
    TTree* photon_origin_tree;
    struct photon_origin_data* photon_origin;
};
