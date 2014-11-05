#!/usr/bin/env python

################################################
#
# This script calculates significance with LEE
# and plots the p-value plot
#
#   - input data from ASCII files
#   
# Usage:
#       ./plots.py -p lee -t '2.692381' -d zprime_significance_714ee_746mumu_22-Jun-2011_20-49-11/ -f pdf
#         
# Gena Kukartsev, 2011
#
################################################

import ROOT
import fileinput
import math
import os
from plotter import *
from ROOT import TMath
from ROOT import TCanvas
from ROOT import TH1F
from ROOT import TLine


def compare_columns(a, b):
    # sort on ascending index 1, then 0
    return cmp(a[1], b[1]) or cmp(a[0], b[0])


    
def plotAscii(filename, columnNumber, plot_format = 'png'):
    #
    # Plots a histogram from a specified column of numbers
    # in an input ascii file
    #

    legend = '[plotAscii]:'

    title = 'S+B pseudoexperiments'

    _h = TH1F(title, title, 20, 0, 10)

    with open(filename) as file:

        lines=file.readlines()
                
        for line in lines:
        
            _words = line.strip().split()
        
            if _words[0][0] == '#':
                continue

            _h.Fill(float(_words[columnNumber]))


    c = TCanvas("c", "c", 600, 400)
    c.SetFrameLineWidth(2)
    c.SetRightMargin(0.05)
    c.cd()

    _h.SetLineWidth(2)
    _h.SetLineColor(ROOT.kBlue+2)
    _h.SetFillStyle(1001)
    _h.SetFillColor(ROOT.kBlue-9)
    _h.Draw("")

    #_l = TLine(_p,0,_p,_h.GetMaximum() * 0.9)
    #_l.SetLineWidth(2)
    #_l.SetLineColor(ROOT.kRed)
    #_l.Draw("same")
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.045)
    latex.SetTextAlign(5) # align left
    #latex.DrawLatex(0.45,0.45,"#font[62]{p-value = "+'%.2f'%_pval+"}")
    #latex.DrawLatex(0.45,0.4,"#font[62]{Z = "+'%.1f'%_z+"}")

    # x axis
    latex.SetTextAlign(31) # align left
    latex.DrawLatex(0.95,0.01, 'local significance')

    # y axis
    latex.SetTextAlign(31) # align left
    latex.SetTextAngle(90)
    latex.DrawLatex(0.04,0.95, 'number of pseudo-experiments')

    # legend
    xLegend = 0.65
    yLegend = 0.75
    legendWidth = 0.3
    legendHeight = 0.15
    tLegend = TLegend(xLegend, yLegend,
                      xLegend + legendWidth, yLegend + legendHeight)
    #tLegend.AddEntry(_h, "pseudo-experiments", 'l')
    #tLegend.AddEntry(_l, "observed data", 'l')
    #tLegend.Draw()

    # CMS preliminary
    latex.SetTextAngle(0)
    #latex.DrawLatex(0.9,0.929,"#font[62]{CMS preliminary, #int #font[42]{L}dt = "+'%.1ffb^{-1} (#mu^{+}#mu^{-})'%intLumiMumu+"}")
    #latex.DrawLatex(0.9,0.929,"#font[62]{CMS preliminary, #int #font[42]{L}dt = "+'%.1ffb^{-1} (ee)'%intLumiEe+"}")
    #latex.DrawLatex(0.9,0.929,"#font[62]{CMS preliminary, #int #font[42]{L}dt = "+'%.1ffb^{-1} (#mu^{+}#mu^{-}), %.1fpb^{-1} (ee)'%(intLumiMumu,intLumiEe)+"}")
    #
    #latex.DrawLatex(0.9,0.929,"#font[62]{CMS, #int #font[42]{L}dt = "+'%.1ffb^{-1} (#mu^{+}#mu^{-})'%intLumiMumu+"}")
    #latex.DrawLatex(0.9,0.929,"#font[62]{CMS, #int #font[42]{L}dt = "+'%.1ffb^{-1} (ee)'%intLumiEe+"}")
    #latex.DrawLatex(0.9,0.929,"#font[62]{CMS, #int #font[42]{L}dt = "+'%.1ffb^{-1} (#mu^{+}#mu^{-}), %.1fpb^{-1} (ee)'%(intLumiMumu,intLumiEe)+"}")
    
    c.SaveAs("ascii."+plot_format)
    
