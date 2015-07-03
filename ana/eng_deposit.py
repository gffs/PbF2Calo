# -*- coding: utf-8 -*-

import math
from ROOT import *
from array import array
import glob

gROOT.SetBatch(1)

gStyle.SetOptStat(0)
gStyle.SetTitleOffset(1.4, "xy")
gStyle.SetTitleOffset(1.0, "z")
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
        self.fl_list = glob.glob('../build/' + fname + '/calo_hits_*.root')

        print self.fl_list

        self.h_eng_long = TH1F("eng_long_" + fname, "eng_long", 110, -10, 210)
        self.h_eng_rad = TH1F("eng_rad_" + fname , "eng_rad", 100, -10, 90)
        self.h_pht_long = TH1F("pht_long_" + fname, "pht_long", 110, -10, 210)
        self.h_pht_rad = TH1F("pht_pos_" + fname, "pht_pos",  100, -10, 90)

    def crunch(self):
        for fname in self.fl_list:
            mc_fl = TFile(fname, 'r')
            eng_tree = mc_fl.Get("eng")
            pht_tree = mc_fl.Get("pht_org")

            eng_tree.Draw("eng_dep.pos_x >> h1(110, -10, 210)", 
                "eng_dep.eng * (eng_dep.pos_x > 0 && eng_dep.pos_x < 200)")
            self.h_eng_long.Add(gDirectory.Get("h1"))

            pht_tree.Draw("pht_dep.pos_x >> h2(110, -10, 210)", 
                "pht_dep.pos_x > 0 && pht_dep.pos_x < 200")
            self.h_pht_long.Add(gDirectory.Get("h2"))

            eng_tree.Draw("sqrt(eng_dep.pos_y**2 + eng_dep.pos_z**2) >> h3(100, -10, 90)", 
                "eng_dep.eng * (eng_dep.pos_x > 0 && eng_dep.pos_x < 200)")
            self.h_eng_rad.Add(gDirectory.Get("h3"))

            pht_tree.Draw("sqrt(pht_dep.pos_y**2 + pht_dep.pos_z**2) >> h4(100, -10, 90)", 
                "pht_dep.pos_x > 0 && pht_dep.pos_x < 200")
            self.h_pht_rad.Add(gDirectory.Get("h4"))

            mc_fl.Close()

    def moliere_radius(self):
        label = ["energy", "photon"]
        mol_rad = []
        for h in [self.h_eng_rad, self.h_pht_rad]:
            bin_from = h.FindBin(0.001); bin_to = h.FindBin(199.999)
            total = float(h.Integral(bin_from, bin_to))
            bin_mol = bin_from
            for bin in range(bin_from, bin_to):
                if h.Integral(bin_from, bin) / total > 0.9:
                    bin_mol = bin; break

            mol_r = (0.9 - h.Integral(bin_from, bin_mol - 1) / total) \
                / float(h.Integral(bin_from, bin_mol - 1) / total - h.Integral(bin_from, bin_mol) / total) \
                * (h.GetBinCenter(bin_mol) - h.GetBinCenter(bin_mol - 1)) \
                + h.GetBinCenter(bin_mol - 1)

            mol_rad.append(mol_r)

        for i in range(len(mol_rad)):
            print "Moliere radius in %s space: %.2f mm" % (label[i], mol_rad[i])

if __name__ == '__main__':
    pos30 = MCRun('.', 3.0)
    pos30.crunch()

    pos30.moliere_radius()

    pos30.h_eng_long.Scale(1.0 / pos30.h_eng_long.Integral())
    pos30.h_eng_long.SetTitle(";position [mm];")
    pos30.h_eng_long.SetLineColor(c_blue)
    pos30.h_eng_long.SetMarkerColor(c_blue)
    pos30.h_eng_long.SetMarkerStyle(1)
    pos30.h_eng_long.GetOption()

    pos30.h_pht_long.Scale(1.0 / pos30.h_pht_long.Integral())
    pos30.h_pht_long.SetTitle(";position [mm];")
    pos30.h_pht_long.SetLineColor(c_red)
    pos30.h_pht_long.SetMarkerColor(c_red)
    pos30.h_pht_long.SetMarkerStyle(1)
    pos30.h_pht_long.GetOption()

    pos30.h_eng_rad.Scale(1.0 / pos30.h_eng_rad.Integral())
    pos30.h_eng_rad.SetTitle(";radial distance from beam [mm];")
    pos30.h_eng_rad.SetLineColor(c_blue)
    pos30.h_eng_rad.SetMarkerColor(c_blue)
    pos30.h_eng_rad.SetMarkerStyle(1)
    pos30.h_eng_rad.GetOption()

    pos30.h_pht_rad.Scale(1.0 / pos30.h_pht_rad.Integral())
    pos30.h_pht_rad.SetTitle(";radial distance from beam [mm];")
    pos30.h_pht_rad.SetLineColor(c_red)
    pos30.h_pht_rad.SetMarkerColor(c_red)
    pos30.h_pht_rad.SetMarkerStyle(1)
    pos30.h_pht_rad.GetOption()

    c = TCanvas()
    pos30.h_pht_long.Draw("h")
    pos30.h_eng_long.Draw("h same")
    c.Print('eng_pht_long.pdf', 'pdf')

    c2 = TCanvas()
    c2.SetLogy()
    pos30.h_pht_rad.GetXaxis().SetRangeUser(-5, 35)
    pos30.h_pht_rad.Draw("h")
    pos30.h_eng_rad.Draw("h same")
    c2.Print('eng_pht_rad.pdf', 'pdf')

