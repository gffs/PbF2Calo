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

c_blue = TColor.GetColor("#301070")
c_lblue = TColor.GetColor("#80b0f0")
c_red = TColor.GetColor("#703010")
c_lred = TColor.GetColor("#f07080")
c_cyan = TColor.GetColor("#005050")

class MCRun:
    def __init__(
            self,
            fname,
            eng = 3.0,
        ):
        self.eng = eng
        self.fl_list = glob.glob('../build/data/' + fname + '/calo_hits_*.root')
        self.h_pht_angle = TH1F("pht_angle" + fname, "pht_angle", 90, 0, 180)

    def crunch(self):
        for fname in self.fl_list:
            mc_fl = TFile(fname, 'r')
            eng_tree = mc_fl.Get("eng")
            pht_tree = mc_fl.Get("pht")

            #pht_tree.Draw("atan2(sqrt(pht_dep.mom_y**2 + pht_dep.mom_z**2), pht_dep.mom_x) * 180.0 / TMath::Pi()  >> h3(90, 0, 180)")
            pht_tree.Draw("atan2(sqrt(pht_dep.mom_y**2 + pht_dep.mom_z**2), pht_dep.mom_x) * 180.0 / TMath::Pi() >> h3(90, 0, 180)", 
                    "pht_dep.pos_y > 20 || pht_dep.pos_z > 20")
            self.h_pht_angle.Add(gDirectory.Get("h3"))

            mc_fl.Close()


if __name__ == '__main__':
    pos30 = MCRun('pos_30gev', 3.0) # 100
    #pos30 = MCRun('pos_30gev_take2', 3.0) # 500
    pos20 = MCRun('pos_20gev', 2.0) # 200
    pos10 = MCRun('pos_10gev', 1.0) # 500

    for pos in [pos30, pos20, pos10]:
        pos.crunch()

    pos30.h_pht_angle.SetTitle(";angle [deg];")
    pos30.h_pht_angle.Scale(1.0 / pos30.h_pht_angle.Integral())
    pos30.h_pht_angle.SetLineColor(c_blue)
    pos30.h_pht_angle.SetMarkerColor(c_blue)
    pos30.h_pht_angle.GetXaxis().SetRangeUser(0, 180)

    pos20.h_pht_angle.Scale(1.0 / pos20.h_pht_angle.Integral() / 2.0)
    pos20.h_pht_angle.SetLineColor(c_cyan)

    pos10.h_pht_angle.Scale(1.0 / pos10.h_pht_angle.Integral() / 5.0)
    pos10.h_pht_angle.SetLineColor(c_red)

    c = TCanvas()

    pos30.h_pht_angle.Draw("h")
    pos20.h_pht_angle.Draw("same h")
    pos10.h_pht_angle.Draw("same h")

    c.Print('pht_angle.pdf', 'pdf')


