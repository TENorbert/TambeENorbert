# This Simple Code Overlays Histograms Plots  from Different Files
# Cannot DO IT with C++ because It root chokes especially when Number bins are 
# not the same.
## Aurthor Tambe E. Norbert a.k.a TEN (norbert@physics.umn.edu)

#!/usr/bin/env python
""" OverLay Myhist.png from several  Directories identical structure!"""
__version__ = "1.0"


import sys
import optparse
import shutil
import os
import re
from ROOT import *
from array import array

# ROOT general options
gROOT.SetBatch(kTRUE)
gROOT.SetStyle("Plain")
gStyle.SetOptStat("emruo")

gStyle.SetCanvasColor(kWhite)  #   // background is no longer mouse-dropping white
gStyle.SetPalette(1) #           // blue to red false color palette. Use 9 for b/w
gStyle.SetCanvasBorderMode(0) #     // turn off canvas borders
gStyle.SetPadBorderMode(0)
gStyle.SetPaintTextFormat("5.2f") #  // What precision to put numbers if plotted with "TEXT"

#  // For publishing:
gStyle.SetLineWidth(2)
gStyle.SetTextSize(1.1)
gStyle.SetLabelSize(0.06,"xy")
gStyle.SetTitleSize(0.06,"xy")
gStyle.SetTitleOffset(1.2,"x")
gStyle.SetTitleOffset(1.0,"y")
#gStyle.SetPadTopMargin(0.1)
#gStyle.SetPadRightMargin(0.1)
gStyle.SetPadBottomMargin(0.16)
gStyle.SetPadLeftMargin(0.12)

# Get Files!
f1 =  TFile("lessThan3JetsPhoton2011B.root")
f2 =  TFile("MoreThan3JetsPhoton2011B.root")
f3 =  TFile("MCgmsbctau250lambda100.root")

#Get input directories
#d1 = TDirectory("/afs/cern.ch/user/t/tambe/www/GMSBAnalysis/NJetsEffect/NewPlotsMay22/MoreThan3Jets","","",)
#d2 = TDirectory("/afs/cern.ch/user/t/tambe/www/GMSBAnalysis/NJetsEffect/NewPlotsMay22/LessThan3Jets","","",)
#d3 = TDirectory("/afs/cern.ch/user/t/tambe/www/GMSBAnalysis/NJetsEffect/NewPlotsMay22/GmsB250cmMC","","",)


ph_time1 = f1.Get("NeutKin/h_Time")
ph_time2 = f2.Get("NeutKin/h_Time")
ph_time3 = f3.Get("NeutKin/h_Time") 
#Event Met
evt_met1 = f1.Get("NeutKin/evt_met") 
evt_met2 = f2.Get("NeutKin/evt_met")
evt_met3 = f3.Get("NeutKin/evt_met")

#  Mean Time
#tA_EE_low6 = f6.Get("single-bias/timeVsAoSigmaHighEElog_1")
#tA_EE_low7 = f7.Get("single-bias/timeVsAoSigmaHighEElog_1")

#  Sigma of Time 
#sA_EE_low4 = f4.Get("single-bias/timeVsAoSigmaHighEElog_2")
#sA_EE_low5 = f5.Get("single-bias/timeVsAoSigmaHighEElog_2")
#sA_EE_low6 = f6.Get("single-bias/timeVsAoSigmaHighEElog_2")
#sA_EE_low7 = f7.Get("single-bias/timeVsAoSigmaHighEElog_2")



# Choose Line Colors
ph_time1.SetLineColor(6)
ph_time2.SetLineColor(9)
ph_time3.SetLineColor(12)

evt_met1.SetLineColor(6)
evt_met2.SetLineColor(9)
evt_met3.SetLineColor(12)


ph_time1.SetLineWidth(1)
ph_time2.SetLineWidth(2)
ph_time3.SetLineWidth(3)


evt_met1.SetLineWidth(1)
evt_met2.SetLineWidth(2)
evt_met3.SetLineWidth(3)
#sA_EE_low3.SetLineColor(4)
#sA_EE_low4.SetLineColor(5)
#sA_EE_low5.SetLineColor(6)
#sA_EE_low6.SetLineColor(7)
#sA_EE_low7.SetLineColor(8)

# Choose Fill Colors
#ph_time1.SetFillColor(6)
#ph_time2.SetFillColor(9)
#ph_time3.SetFillColor(12)

#evt_met1.SetFillColor(6)
#evt_met2.SetFillColor(9)
#evt_met3.SetFillColor(12)

#tA_EBMod4.SetFillColor(1)
#tA_EE_low1.SetFillColor(2)
#tA_EE_low2.SetFillColor(3)
#tA_EE_low3.SetFillColor(4)
#tA_EE_low4.SetFillColor(5)
#tA_EE_low5.SetFillColor(6)
#tA_EE_low6.SetFillColor(7)
#tA_EE_low7.SetFillColor(8)

#sA_EBMod4.SetFillColor(1)
#sA_EE_low1.SetFillColor(2)
#sA_EE_low2.SetFillColor(3)
#sA_EE_low3.SetFillColor(4)
#sA_EE_low4.SetFillColor(5)
#sA_EE_low5.SetFillColor(6)
#sA_EE_low6.SetFillColor(7)
#sA_EE_low7.SetFillColor(8)

#Choose Marker Style!
ph_time1.SetMarkerStyle(6)
ph_time2.SetMarkerStyle(9)
ph_time3.SetMarkerStyle(12)

evt_met1.SetMarkerStyle(6)
evt_met2.SetMarkerStyle(9)
evt_met3.SetMarkerStyle(12)

#tA_EBMod4.SetMarkerStyle(1)
#tA_EE_low1.SetMarkerStyle(2)
#tA_EE_low2.SetMarkerStyle(3)
#tA_EE_low3.SetMarkerStyle(4)
#tA_EE_low4.SetMarkerStyle(5)
#tA_EE_low5.SetMarkerStyle(6)
#tA_EE_low6.SetMarkerStyle(7)
#tA_EE_low7.SetMarkerStyle(8)
 #SIgma
#sA_EBMod4.SetMarkerStyle(1)
#sA_EE_low1.SetMarkerStyle(2)
#sA_EE_low2.SetMarkerStyle(3)
#sA_EE_low3.SetMarkerStyle(4)
#sA_EE_low4.SetMarkerStyle(5)
#sA_EE_low5.SetMarkerStyle(6)
#sA_EE_low6.SetMarkerStyle(7)
#sA_EE_low7.SetMarkerStyle(8)

#SetMaxRange XY Axis
#tA_EBMod4.GetYaxis().SetRangeUser(-1.0,0.4)
#tA_EBMod4.GetXaxis().SetRangeUser(0.0,10000.0)

ph_time1.GetXaxis().SetTitle("Photon Time(ns)")
ph_time1.GetYaxis().SetTitle("Event Number")

evt_met1.GetXaxis().SetTitle("MET(GeV)")
evt_met2.GetYaxis().SetTitle("Event Number")

#sA_EBMod4.GetYaxis().SetRangeUser(0.0,3.5)
#sA_EBMod4.GetXaxis().SetRangeUser(0.0,10000.0)
#sA_EBMod4.GetXaxis().SetTitle("Amplitude(ADC Counts)")
#sA_EBMod4.GetYaxis().SetTitle("#sigma Time(ns)")


# Draw Plots now
# *************************************************************************************
c1 = TCanvas("c1", "Photon Time", 800, 800)
c1.cd()
# ht->GetXaxis()->SetTitle("Energy(GeV)");
#  ht->GetYaxis()->SetTitle("Mean Time(ns)");
#  ht->GetYaxis()->SetRangeUser(-0.8,0.3);
#  ht->GetXaxis()->SetRangeUser(0, 10000);

ph_time1.SetTitle("Photon Time Distribution")
#tA_EBMod4.SetTitle("Mean Time Vs Amplitude Stability")

ph_time1.Draw()
ph_time2.Draw("sames")
ph_time3.Draw("sames")

#tA_EBMod4.Draw()
#tA_EE_low1.Draw("sames")
#tA_EE_low2.Draw("sames")
#tA_EE_low3.Draw("sames")
#tA_EE_low4.Draw("sames")
#tA_EE_low5.Draw("sames")
#tA_EE_low6.Draw("sames")
#tA_EE_low7.Draw("sames")

#Add Legend
leg1 = TLegend( 0.14, 0.70, 0.47, 0.90)
leg1.SetBorderSize(1)
leg1.SetFillColor(0)
leg1.AddEntry(ph_time1,"< 3Jets + VLIso#gamma + MET>30GeV","l")
leg1.AddEntry(ph_time2,">= 3Jets + VLIso#gamma + MET>30GeV","l")
leg1.AddEntry(ph_time3,"GMSB-c#tau=250mm-#Lambda=100TeV","l")
leg1.SetTextSize(0.018)
leg1.SetHeader("Samples")
#leg1.AddEntry(tA_EE_low3,"1.9 <|#eta|<2.2","l")
#leg1.AddEntry(tA_EE_low4,"2.2 <|#eta|<2.4","l")
#leg1.AddEntry(tA_EE_low5,"2.4 <|#eta|<2.6","l")
#leg1.AddEntry(tA_EE_low6,"2.6 <|#eta|<2.8","l")
#leg1.AddEntry(tA_EE_low7,"2.8 <|#eta|<3.0","l")
leg1.Draw()

# Use for MET Plot 
c2 = TCanvas ( "c2", "Sigma Time Stability", 800, 800)
c2.cd()
#  ht->GetXaxis()->SetTitle("Energy(GeV)");
#  ht->GetYaxis()->SetTitle("#sigma Time(ns)");
#  ht->GetYaxis()->SetRangeUser(0,3.5);
#  ht->GetXaxis()->SetRangeUser(0, 10000);

evt_met1.SetTitle("Missing Energy Distribution") 
evt_met1.Draw()
evt_met2.Draw("sames")
evt_met3.Draw("sames")
#sA_EE_low3.Draw("sames")
#sA_EE_low4.Draw("sames")
#sA_EE_low5.Draw("sames")
#sA_EE_low6.Draw("sames")
#sA_EE_low7.Draw("sames")

# Add Legend


#Add Legend
leg2 = TLegend( 0.35, 0.55, 0.73, 0.77)
leg2.SetBorderSize(1)
leg2.SetFillColor(0)
leg2.AddEntry(evt_met1,"< 3Jets + VLIso#gamma + MET>30GeV","l")
leg2.AddEntry(evt_met2,">= 3Jets + VLIso#gamma + MET>30GeV","l")
leg2.AddEntry(evt_met3,"GMSB-c#tau=250mm-#Lambda=100TeV","l")
leg2.SetHeader("Samples")
leg2.SetTextSize(0.02)
#leg2 = TLegend( 0.8, 0.55, 0.98,0.77)
#leg2.SetBorderSize(1)
#leg2.SetFillColor(0)
#leg2.AddEntry(sA_EBMod4,"EB Mod4","l")
#leg2.AddEntry(sA_EE_low1,"1.5 <|#eta|<1.7","l")
#leg2.AddEntry(sA_EE_low2,"1.7 <|#eta|<1.9","l")
#leg2.AddEntry(sA_EE_low3,"1.9 <|#eta|<2.2","l")
#leg2.AddEntry(sA_EE_low4,"2.2 <|#eta|<2.4","l")
#leg2.AddEntry(sA_EE_low5,"2.4 <|#eta|<2.6","l")
#leg2.AddEntry(sA_EE_low6,"2.6 <|#eta|<2.8","l")
#leg2.AddEntry(sA_EE_low7,"2.8 <|#eta|<3.0","l")
leg2.Draw()
  
# Save Plots
c1.SaveAs("PhotonTimeComparing.png")
c1.SetLogy(1)
c1.SaveAs("LogPhotonTimeComparing.png")


c2.SaveAs("EventMet.png")
c2.SetLogy(1)
c2.SaveAs("LogEventMet.png")




