# -*- coding: utf-8 -*-

import math
from ROOT import *
from array import array
import glob

gROOT.SetBatch(1)

gStyle.SetOptStat(0)
gStyle.SetTitleOffset(1.4, "xyz")
gStyle.SetHistLineWidth(4)
gStyle.SetStripDecimals(kFALSE)

c_blue = TColor.GetColor("#102090")
c_lblue = TColor.GetColor("#80b0f0")
c_red = TColor.GetColor("#902010")
c_lred = TColor.GetColor("#f07080")

class MCRun:
    def __init__(
            self,
            fname,
            eng = 3.0,
        ):
        self.eng = eng
        self.fl_list = glob.glob('../build/data/' + fname + '/calo_hits_*.root')
        self.h_eng_time = TH1F("eng_time" + fname, "eng_time", 50, 0, 2)
        self.h_eng_pos = TH1F("eng_pos" + fname , "eng_pos", 50, -50, 200)
        self.h_pht_time = TH1F("pht_time" + fname, "pht_time", 50, 0, 2)
        self.h_pht_pos = TH1F("pht_pos" + fname, "pht_pos", 50, -50, 200)

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

            eng_tree.Draw("eng_dep.time_g >> h1(50, 0, 2)")
            self.h_eng_time.Add(gDirectory.Get("h1"))

            eng_tree.Draw("eng_dep.pos_x >> h2(50,-50,200)", "eng.eng")
            self.h_eng_pos.Add(gDirectory.Get("h2"))

            pht_tree.Draw("pht_dep.time_g >> h3(50, 0, 2)")
            self.h_pht_time.Add(gDirectory.Get("h3"))

            pht_tree.Draw("pht_dep.pos_x >> h4(50,-50,200)")
            self.h_pht_pos.Add(gDirectory.Get("h4"))

            mc_fl.Close()


if __name__ == '__main__':
    #pos30 = MCRun('pos_30gev', 3.0)
    pos30 = MCRun('pos_30gev_take2', 3.0)
    pos30.crunch()

 
    pos30.h_eng_pos.Scale(1.0 / pos30.h_eng_pos.Integral())
    pos30.h_eng_pos.SetTitle(";position [mm];")
    pos30.h_eng_pos.SetLineColor(c_blue)
    pos30.h_eng_pos.SetMarkerColor(c_blue)
    pos30.h_eng_pos.SetMarkerStyle(1)
    pos30.h_eng_pos.GetOption()

    pos30.h_pht_pos.Scale(1.0 / pos30.h_pht_pos.Integral())
    pos30.h_pht_pos.SetTitle(";position [mm];")
    pos30.h_pht_pos.SetLineColor(c_red)
    pos30.h_pht_pos.SetMarkerColor(c_red)
    pos30.h_pht_pos.GetOption()

    c = TCanvas()

    pos30.h_eng_pos.Draw("h")
    pos30.h_pht_pos.Draw("h same")

    c.Print('eng_pht_pos.pdf', 'pdf')


