#include "G4Event.hh"
#include "G4Track.hh"
#include "MuonTrackSniffer.h"
#include "TTree.h"
#include "TFile.h"

#include "iostream"

MuonTrackSniffer::MuonTrackSniffer(const json11::Json& cfg, TFile& root_file):
    ProductBase(cfg),
    muon_track_q(new std::queue<struct muon_track_data>()),
    muon_track_tree(0),
    muon_track(new muon_track_data())
{
    root_file.cd();
    muon_track_tree = new TTree("muon_track_sniffer", "muon_track_sniffer");
    muon_track_tree->Branch("muon_track", &muon_track->pos_x,
            "pos_x/F:pos_y:pos_z:time_g:mom_x:mom_y:mom_z:spin_x:spin_y:spin_z:ev_num/i:trk_num", 10*1024*1024);
    muon_track_tree->Fill();
    muon_track_tree->Reset();
}


MuonTrackSniffer::~MuonTrackSniffer()
{
    delete muon_track;
    delete muon_track_q;
}

int MuonTrackSniffer::FillTree() const
{
    while (!muon_track_q->empty()) {
        *muon_track = muon_track_q->front();
        muon_track_q->pop();
        muon_track_tree->Fill();
    }

    return 0;
}

int MuonTrackSniffer::Fill(const G4Track& t, const G4Event& e) const
{
    const G4StepPoint* s = t.GetStep()->GetPostStepPoint();
    const G4ThreeVector& pos = s->GetPosition();
    const G4ThreeVector& mom = s->GetMomentum();
    const G4ThreeVector& pol = s->GetPolarization();

    struct muon_track_data mtd {
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(s->GetGlobalTime()),
        static_cast<float>(mom.x()),
        static_cast<float>(mom.y()),
        static_cast<float>(mom.z()),
        static_cast<float>(pol.x()),
        static_cast<float>(pol.y()),
        static_cast<float>(pol.z()),
        static_cast<unsigned int>(e.GetEventID()),
        static_cast<unsigned int>(t.GetTrackID())
    };

    muon_track_q->push(mtd);

    return 0;
}
