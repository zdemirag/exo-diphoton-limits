#!/usr/bin/python

import sys,string,time,os,glob,re

import ROOT, sys, os, string, re
from ROOT import TCanvas, TFile, TProfile, TNtuple, TH1F, TH1D, TH2F,TF1, TPad, TPaveLabel, TPaveText, TLegend, TLatex, THStack, TGraph, TGraphErrors, TGraphAsymmErrors, TVectorD
from ROOT import gROOT, gBenchmark, gRandom, gSystem, Double, gPad

from array import array

import math
from math import *

from tdrStyle import *
setTDRStyle()

#mass = ["1500", "1750", "2000", "2250", "2500", "2750", "3000", "3250" ,"3500"]

mass = array("d",[500,550,650,700,750,800,850,900,950,1000,1050, 1100,1150, 1200,1250,1300,1350,1400,1500, 1750, 2000, 2250, 2500, 2750, 3000, 3250 ,3500])

expected = {}
s_expected = {}

observed = {}
s_observed = {}

exp =    array("d",[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
obs =    array("d",[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
exp68H = array("d",[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
exp68L = array("d",[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
exp95H = array("d",[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
exp95L = array("d",[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])

masserr = array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
obserr =  array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
experr =  array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])
#exp = array("d",[0,0,0,0,0,0,0,0])
#obs = {}
i=0 
for m in mass:
    expected[m] = []
    s_expected[m] = []
    observed[m] = []
    s_observed[m] = []
    fexpected = open("expected_c_001.txt","r")
    fobserved = open("observed_c_001.txt","r")
    for line in fexpected:
        #print line
        #print str(m)
        m_str = str(m)[:-2]
        if line.startswith(m_str):
            #print str(m)
        #if line.startswith(str(m)):
            #print line
            expected[m].append("%.6f" % float(line.split()[1]))
        elif line.startswith("  " + str(m)):
            #print str(m)
        #if line.startswith(""+str(m)):
            #print line
            expected[m].append("%.6f" % float(line.split()[1]))
        s_expected[m] = sorted(expected[m])
    for line_ob in fobserved:
        if line_ob.startswith(m_str):
            #print line_ob
        #if line_ob.startswith(str(m)):
            observed[m].append("%.6f" % float(line_ob.split()[1]))
        elif line_ob.startswith("  " + str(m)):
            #print line_ob
            #if line_ob.startswith(str(m)):
            observed[m].append("%.6f" % float(line_ob.split()[1]))
        s_observed[m] = sorted(observed[m])
    #print "# of toys: ", len(s_expected[m])
    #print "mass: ", m , "expected: " ,  s_expected[m][int(len(s_expected[m])/2)], " observed: " ,  s_observed[m][int(len(s_observed[m])/2)]
#(float(s_observed[m][0]) + float(s_observed[m][1]))/2
    #print "mass: ", m, "%68 up: ", s_expected[m][int(len(s_expected[m])*.84)], "%68 down: ", s_expected[m][int(len(s_expected[m])*.16)]
    #print "mass: ", m, "%95 up: ", s_expected[m][int(len(s_expected[m])*.975)], "%95 down: ", s_expected[m][int(len(s_expected[m])*.025)]


    print m, "&",  s_expected[m][int(len(s_expected[m])/2)], "&", s_observed[m][int(len(s_observed[m])/2)], "\\"

    exp[i] = float(s_expected[m][len(s_expected[m])/2])
    obs[i] = float(s_observed[m][len(s_observed[m])/2])
             #(float(s_observed[m][0]) + float(s_observed[m][1]))/2#
    

    exp68H[i] = abs(float(s_expected[m][len(s_expected[m])/2]) - float(s_expected[m][int(len(s_expected[m])*.84)]))
    exp68L[i] = abs(float(s_expected[m][len(s_expected[m])/2]) - float(s_expected[m][int(len(s_expected[m])*.16)]))
    exp95H[i] = abs(float(s_expected[m][len(s_expected[m])/2]) - float(s_expected[m][int(len(s_expected[m])*.975)]))
    exp95L[i] = abs(float(s_expected[m][len(s_expected[m])/2]) - float(s_expected[m][int(len(s_expected[m])*.025)]))





    i = i+1


#print "mass",mass
#print "exp", exp
#print "obs", obs
#print "68H", exp68H
#print "68L", exp68L
#print "95H", exp95H
#print "95L", exp95L


massv = TVectorD(len(mass),mass)
expv = TVectorD(len(mass),exp)
obsv = TVectorD(len(mass),obs)
exp68Hv = TVectorD(len(mass),exp68H)
exp68Lv = TVectorD(len(mass),exp68L)
exp95Hv = TVectorD(len(mass),exp95H)
exp95Lv = TVectorD(len(mass),exp95L)

masserrv = TVectorD(len(mass),masserr)
obserrv = TVectorD(len(mass),obserr)
experrv = TVectorD(len(mass),experr)


theory_a = array("d",[0.01020000037,0.002020000014,0.0005280000041,0.000159999996,5.420000161e-05,1.852999958e-05,4.702999945e-07])
theory_v = TVectorD(7,theory_a)
mass_a = array("d",[750,1000,1250,1500,1750,2000,3000])
mass_v = TVectorD(7,mass_a)
err_a = array("d",[0,0,0,0,0,0,0,0])
err_v = TVectorD(7,err_a)


  


## theory_a = array("d",[0.0006434760289,0.0005751010031,0.0005231099785,0.0005344069796,0.0005340549978])
## theory_v = TVectorD(5,theory_a)
## mass_a = array("d",[1750,2000,2500,2750,3000])
## mass_v = TVectorD(5,mass_a)
## err_a = array("d",[0,0,0,0,0,0])
## err_v = TVectorD(5,err_a)


theory = TGraphAsymmErrors(mass_v,theory_v,err_v,err_v,err_v,err_v)
theory.SetLineColor(4)
theory.SetLineStyle(9)
theory.SetLineWidth(3)    


observed_p = TGraphAsymmErrors(massv,obsv,masserrv,masserrv,obserrv,obserrv)
observed_p.SetLineColor(ROOT.kBlack)    
observed_p.SetLineWidth(2)
observed_p.SetMarkerStyle(20)    
expected_p = TGraphAsymmErrors(massv,expv,masserrv,masserrv,experrv,experrv)
expected_p.SetLineColor(ROOT.kBlack)    
expected_p.SetLineWidth(2)    
expected_p.SetLineStyle(2)

expected68 = TGraphAsymmErrors(massv,expv,masserrv,masserrv,exp68Lv,exp68Hv)
expected68.SetFillColor(ROOT.kGreen)
expected95 = TGraphAsymmErrors(massv,expv,masserrv,masserrv,exp95Lv,exp95Hv)
expected95.SetFillColor(ROOT.kYellow)

c4 = TCanvas("c4","Diphoton c = 01 Limits", 1000, 800)

c4.SetBottomMargin(0.15)
c4.SetRightMargin(0.06)

#c4.SetLogy(1)

#expected95.SetMinimum(0.0001);
expected95.SetMaximum(0.0056);
expected95.Draw("a3") 
expected95.GetXaxis().SetTitle("Diphoton Mass [GeV/c^{2}]")
expected95.GetYaxis().SetTitle("#sigma(G_{RS} #rightarrow #gamma #gamma)[pb]")
expected68.Draw("3same")
expected_p.Draw("csame") 
observed_p.Draw("cpsame")
theory.Draw("same")



latex = TLatex()
latex.SetNDC()
latex.SetTextSize(0.04)
latex.SetTextAlign(31) # align right
latex.DrawLatex(0.45, 0.95, "CMS Preliminary");

latex2 = TLatex()
latex2.SetNDC()
latex2.SetTextSize(0.04)
latex2.SetTextAlign(31) # align right
latex2.DrawLatex(0.87, 0.95, "19.37 fb^{-1} at #sqrt{s} = 8 TeV")

latex4 = TLatex()
latex4.SetNDC()
latex4.SetTextSize(0.04)
latex4.SetTextAlign(31) # align right
latex4.DrawLatex(0.80, 0.87, "");

legend = TLegend(.53,.637,.84,.84)
legend . AddEntry(observed_p , '95% C.L. Observed', "lp")
legend . AddEntry(expected_p , '95% C.L. Expected', "l")
legend . AddEntry(expected68 , '#pm 1#sigma Expected', "f")
legend . AddEntry(expected95 , '#pm 2#sigma Expected', "f")
legend . AddEntry(theory, "G_{KK} c = 0.01","l")

legend.SetShadowColor(0);
legend.SetFillColor(0);
legend.SetLineColor(0);
legend.Draw()        


c4.SaveAs("limit_c001.root")

raw_input("Press Enter to continue...")

h_expected=TH1F("expected","expected",1000,10e-5,10e-3)
    
for j in range(0,len(s_expected[600])):
    #print  float(s_expected[m][1])
    h_expected.Fill(float(s_expected[600][j]))


#h_expected.Draw()


#raw_input("Press Enter to continue...")
