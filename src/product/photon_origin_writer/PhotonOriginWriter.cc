#include "G4Event.hh"
#include "G4MultiSteppingAction.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"

#include "PhotonOriginSteppingAction.h"
#include "PhotonOriginWriter.h"
#include "TTree.h"
#include "TFile.h"

#include "iostream"

PhotonOriginWriter::PhotonOriginWriter(const json11::Json& cfg, TFile& root_file):
    ProductBase(cfg),
    photon_origin_q(new std::queue<struct photon_origin_data>()),
    photon_origin_tree(0),
    photon_origin(new photon_origin_data())
{
    root_file.cd();
    photon_origin_tree = new TTree("photon_origin", "photon_origin");
    photon_origin_tree->Branch("photon_origin", &photon_origin->pos_x,
            "pos_x/F:pos_y:pos_z:time_g:mom_x:mom_y:mom_z:kin:ev_num/i:par_nm[8]/C:proc_nm[8]/C", 1024*1024);
    photon_origin_tree->Fill();
    photon_origin_tree->Reset();

    G4Region* rs = G4RegionStore::GetInstance()->GetRegion("CalorimeterCrystalArray");
    if (!rs) { return; }

    rs->SetRegionalSteppingAction(new PhotonOriginSteppingAction);
}


PhotonOriginWriter::~PhotonOriginWriter()
{
    delete photon_origin;
    delete photon_origin_q;
}


int PhotonOriginWriter::FillTree() const
{
    while (!photon_origin_q->empty()) {
        *photon_origin = photon_origin_q->front();
        photon_origin_q->pop();
        photon_origin_tree->Fill();
    }

    return 0;
}


int PhotonOriginWriter::Fill(const G4ThreeVector& pos, G4double gtime,
        const G4ThreeVector& mom, G4int evID, G4double kin,
        const G4String& particleName, const G4String& procName) const
{
    struct photon_origin_data po = {
        static_cast<float>(pos.x()),
        static_cast<float>(pos.y()),
        static_cast<float>(pos.z()),
        static_cast<float>(gtime),
        static_cast<float>(mom.x()),
        static_cast<float>(mom.y()),
        static_cast<float>(mom.z()),
        static_cast<float>(kin),
        static_cast<unsigned int>(evID),
        {}, {}
    };

    strncpy(po.par_nm, particleName.c_str(), 7);
    po.par_nm[7] = '\0';
    strncpy(po.proc_nm, procName.c_str(), 7);
    po.proc_nm[7] = '\0';

    photon_origin_q->push(po);
    return 0;
}
