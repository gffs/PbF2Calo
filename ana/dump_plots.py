# -*- coding: utf-8 -*-

import math
from ROOT import *
from array import array
import glob

gROOT.SetBatch(1)

gStyle.SetOptStat(0)
gStyle.SetTitleOffset(1.4, "xyz")
gStyle.SetMarkerStyle(8)
gStyle.SetMarkerSize(3)
gStyle.SetHistLineWidth(3)
#gStyle.SetErrorX(0)
gStyle.SetStripDecimals(kFALSE)
gROOT.UseCurrentStyle()
gROOT.ForceStyle()


class MCRun:
    def __init__(
            self,
            fname,
            eng = 3.0,
        ):
        self.eng = eng
        self.fl_list = glob.glob('../build/data/' + fname + '/calo_hits_*.root')
        self.h_eng_time = TH1F("eng_time" + fname, "eng_time", 200, 0, 2)
        self.h_eng_pos = TH1F("eng_pos" + fname , "eng_time", 250, -50, 200)
        self.h_pht_time = TH1F("pht_time" + fname, "pht_time", 200, 0, 2)
        self.h_pht_pos = TH1F("pht_pos" + fname, "pht_pos", 250, -50, 200)

    def crunch(self):
        for fname in self.fl_list:
            mc_fl = TFile(fname, 'r')
            eng_tree = mc_fl.Get("eng")
            pht_tree = mc_fl.Get("pht")

            """
            eng_tree_reader = TTreeReader(eng_tree)
            time_g = TTreeReaderValue("Float_t")(eng_tree_reader, "eng_dep.time_g")
            pos_x = TTreeReaderValue("Float_t")(eng_tree_reader, "eng_dep.pos_x")
            eng = TTreeReaderValue("Float_t")(eng_tree_reader, "eng_dep.eng")
            while eng_tree_reader.Next():
                self.h_eng_time.Fill(time_g.Get()[0])
                self.h_eng_pos.Fill(pos_x.Get()[0], eng.Get()[0])

            """

            eng_tree.Draw("eng.time_g >> h1(200, 0, 2)")
            self.h_eng_time.Add(gDirectory.Get("h1"))

            eng_tree.Draw("eng.pos_x >> h2(250,-50,200)", "eng.eng")
            self.h_eng_pos.Add(gDirectory.Get("h2"))

            pht_tree.Draw("pht.time_g >> h3(200, 0, 2)")
            self.h_pht_time.Add(gDirectory.Get("h3"))

            pht_tree.Draw("pht.pos_x >> h4(250,-50,200)")
            self.h_pht_pos.Add(gDirectory.Get("h4"))

            mc_fl.Close()

        self.h_eng_time.Print()
        self.h_eng_pos.Print()
        self.h_pht_time.Print()
        self.h_pht_pos.Print()

if __name__ == '__main__':
    pos30 = MCRun('pos_30gev', 3.0)
    pos20 = MCRun('pos_20gev', 2.0)

    for dt in [pos30, pos20]:
        dt.crunch()



