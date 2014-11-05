#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"

#include "iostream"
#include "queue"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] <<
            " root_file_to_reconstruct" << std::endl;
        return -1;
    }

    TFile* fl = TFile::Open(argv[1], "r");
    if (!fl) {
        std::cout << "failed to open: \"" << argv[1] << "\"." << std::endl;
    }

    struct eng_recon {
        long ev_num;
        float eng_calo;
        float eng_AlPlate;
        float eng_SiPlate;
        float eng_lost;
    };

    struct ph_recon {
        long ev_num;
        long num_ph;
        long num_ph_3x3;
        long num_ph_3x3_prompt;
    };

    std::queue<eng_recon*> ev_q;
    eng_recon ev = {-1,0,0,0,0};
    std::map<std::string, float*> ev_buf = {
        {"PbF2", &ev.eng_calo},
        {"AlPlate", &ev.eng_AlPlate},
        {"Nusil", &ev.eng_SiPlate},
        {"World", &ev.eng_lost},
        {"OutOfWo", &ev.eng_lost},
    };

    std::queue<ph_recon*> ph_q;
    ph_recon ph = {-1,0,0,0};

    TTreeReader ttr("eng", fl);
    TTreeReaderValue<float> eng(ttr, "eng_dep.eng");
    TTreeReaderArray<char> vol_nm(ttr, "eng_dep.vol_nm");
    TTreeReaderValue<unsigned int> ev_num(ttr, "eng_dep.ev_num");

    TTreeReader ttrp("pht_org", fl);
    TTreeReaderValue<unsigned int> ph_num(ttrp, "pht_dep.ev_num");
    TTreeReaderValue<float> pos_x(ttrp, "pht_dep.pos_x");
    TTreeReaderValue<float> pos_y(ttrp, "pht_dep.pos_y");
    TTreeReaderValue<float> pos_z(ttrp, "pht_dep.pos_z");
    TTreeReaderValue<float> time_g(ttrp, "pht_dep.time_g");

    if (!ttr.GetEntries(true)) {
        std::cout << "the eng tree is empty." << std::endl;
        fl->Close();
        return -1;
    }

    ttr.SetLocalEntry(0);
    ev = {*ev_num, 0,0,0,0};
    ttr.SetLocalEntry(-1);

    while(ttr.Next()) {
        if (*ev_num != ev.ev_num) {
                ev_q.emplace(new eng_recon(ev));
                ev = {*ev_num, 0,0,0,0};
        }
        *ev_buf.at(&vol_nm[0]) += *eng;
    }
    ev_q.emplace(new eng_recon(ev));

    ttrp.SetLocalEntry(0);
    ph = {*ph_num, 0,0,0};
    ttrp.SetLocalEntry(-1);

    while(ttrp.Next()) {
        if (*ph_num != ph.ev_num) {
                ph_q.emplace(new ph_recon(ph));
                ph = {*ph_num, 0,0,0};
        }
        if (*pos_x > 0 && *pos_x < 140) {
            ph.num_ph++;
            if (*pos_y < 37.5 && *pos_y > -37.5 && *pos_z < 37.5 && *pos_z > -37.5) {
                ph.num_ph_3x3++;
                if (*time_g < 5.0) {
                    ph.num_ph_3x3_prompt++;
                }
            }
        }
    }
    ph_q.emplace(new ph_recon(ph));

    fl->Close();

    TFile fl_dump("recon.root", "recreate");
    TTree* recon_tree = new TTree("eng", "eng");
    recon_tree->Branch("eng_dep", &ev.ev_num,
            "ev_num/L:eng_calo/F:eng_AlPlate:eng_SiPlate:eng_lost", 1024*1024);

    while (!ev_q.empty()) {
        ev = *ev_q.front();
        ev_q.pop();
        recon_tree->Fill();
    }

    TTree* ph_recon_tree = new TTree("ph_org", "ph_org");
    ph_recon_tree->Branch("ph_org", &ph.ev_num,
            "ev_num/L:num_ph:num_ph_3x3:num_ph_3x3_prompt", 1024*1024);

    while (!ph_q.empty()) {
        ph = *ph_q.front();
        ph_q.pop();
        ph_recon_tree->Fill();
    }

    recon_tree->Write();
    ph_recon_tree->Write();
    fl_dump.Close();

    return 0;
}

