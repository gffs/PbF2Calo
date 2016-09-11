#pragma once

#include "MuonTrackSnifferData.h"
#include "ProductBase.h"
#include "json11.hpp"
#include <queue>

class G4Event;
class G4Track;
class TFile;
class TTree;

class MuonTrackSniffer: public ProductBase
{
public:
    MuonTrackSniffer(const json11::Json& cfg, TFile& root_file);
    ~MuonTrackSniffer();

    virtual int FillTree() const;
    int Fill(const G4Track& t, const G4Event& e) const;

private:
    std::queue<struct muon_track_data>* muon_track_q;
    TTree* muon_track_tree;
    struct muon_track_data* muon_track;
};
