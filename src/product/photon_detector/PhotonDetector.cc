#include "G4Event.hh"
#include "G4MultiSteppingAction.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"

#include "PhotonDetector.h"
#include "TTree.h"
#include "TFile.h"

#include "iostream"

PhotonDetector::PhotonDetector(const json11::Json& cfg, TFile& root_file):
    ProductBase(cfg),
    photon_detector_q(new std::queue<struct photon_detector_data>()),
    photon_detector_tree(0),
    photon_detector(new photon_detector_data())
{
    root_file.cd();
    photon_detector_tree = new TTree("photon_detector", "photon_detector");

    photon_detector_tree->Branch("photon_detector", &photon_detector->pos_x,
        "pos_x/F:pos_y:pos_z:time_g:mom_x:mom_y:mom_z:pos_org_x:pos_org_y:pos_org_z:mom_org_x:mom_org_y:mom_org_z:ev_num/i:bnc_num", 1024*1024);

    photon_detector_tree->Fill();
    photon_detector_tree->Reset();

    //SiPlate model hardcoded in Physics list for now
}


PhotonDetector::~PhotonDetector()
{
    delete photon_detector;
    delete photon_detector_q;
}


int PhotonDetector::FillTree() const
{
    while (!photon_detector_q->empty()) {
        *photon_detector = photon_detector_q->front();
        photon_detector_q->pop();
        photon_detector_tree->Fill();
    }

    return 0;
}


int PhotonDetector::Fill(const G4ThreeVector& pos, G4double gtime,
        const G4ThreeVector& mom, G4int evID, const G4ThreeVector& pos_org,
        const G4ThreeVector& mom_org, const unsigned short num_bounces) const
{

    struct photon_detector_data pdd = {
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<float>(mom.x()),
        static_cast<float>(mom.y()),
        static_cast<float>(mom.z()),
        static_cast<float>(pos_org.x()),
        static_cast<float>(pos_org.y()),
        static_cast<float>(pos_org.z()),
        static_cast<float>(mom_org.x()),
        static_cast<float>(mom_org.y()),
        static_cast<float>(mom_org.z()),
        static_cast<unsigned int>(evID),
        static_cast<unsigned int>(num_bounces)
    };

    photon_detector_q->push(pdd);
    return 0;
}
