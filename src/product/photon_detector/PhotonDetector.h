#pragma once

#include "PhotonDetectorData.h"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "ProductBase.h"
#include "json11.hpp"
#include <queue>

class TFile;
class TTree;

class PhotonDetector: public ProductBase
{
public:
    PhotonDetector(const json11::Json& cfg, TFile& root_file);
    ~PhotonDetector();

    virtual int FillTree() const;

    int Fill(const G4ThreeVector& pos, G4double gtime,
        const G4ThreeVector& mom, G4int evID, const G4ThreeVector& pos_org,
        const G4ThreeVector& mom_org, const unsigned short num_bounces) const;

private:
    std::queue<struct photon_detector_data>* photon_detector_q;
    TTree* photon_detector_tree;
    struct photon_detector_data* photon_detector;
};
