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

    std::queue<eng_recon*> ev_q;
    eng_recon ev = {-1,0,0,0,0};
    std::map<std::string, float*> ev_buf = {
        {"PbF2", &ev.eng_calo},
        {"AlPlate", &ev.eng_AlPlate},
        {"Nusil", &ev.eng_SiPlate},
        {"World", &ev.eng_lost},
        {"OutOfWo", &ev.eng_lost},
    };

    TTreeReader ttr("eng", fl);
    TTreeReaderValue<float> eng(ttr, "eng_dep.eng");
    TTreeReaderArray<char> vol_nm(ttr, "eng_dep.vol_nm");
    TTreeReaderValue<unsigned int> ev_num(ttr, "eng_dep.ev_num");

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

    recon_tree->Write();
    fl_dump.Close();

    return 0;
}

