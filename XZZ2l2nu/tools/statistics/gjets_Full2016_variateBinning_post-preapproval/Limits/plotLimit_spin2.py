#!/bin/env python
from ROOT import *
from tdrStyle import *
setTDRStyle()
        
from array import array
import ROOT, os, string
import math
from math import *

import sys, os, pwd, commands
from subprocess import *
import optparse, shlex, re

# load signal modules
sys.path.append('./SigInputs')
sys.path.append('./BulkGravXsec')

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

def parseOptions():

    usage = ('usage: %prog [options]\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    parser.add_option("--cutChain", dest="CutChain",default='zpt100met50',help="kinematic cuts on zpt and met")
    parser.add_option("--dyMC",action="store_true", dest="DyMC", default=False,help="whether use DY MC to predict Z+jets MT spectrum")
    parser.add_option("--perBinStatUnc",action="store_true", dest="PerBinStatUnc", default=False,help="whether do per-bin statistical uncertainty")
    parser.add_option("--obs", dest="Observable",default='mT',help="template observable")
    parser.add_option("--unblind",action="store_true", dest="unblind", default=False,help="unblind")

    parser.add_option("-l",action="callback",callback=callback_rootargs)
    parser.add_option("-q",action="callback",callback=callback_rootargs)
    parser.add_option("-b",action="callback",callback=callback_rootargs)

    return parser

def plotLimit(parser):

    mass = array('d',[])
    zeros = array('d',[])

    obs = array('d',[])
    obs_ee = array('d',[])
    obs_mm = array('d',[])

    exp = array('d',[])
    exp_p2 = array('d',[])
    exp_p1 = array('d',[])
    exp_m1 = array('d',[])
    exp_m2 = array('d',[])

    exp_ee = array('d',[])
    exp_mm = array('d',[])

    exp_ee_p2 = array('d',[])
    exp_ee_p1 = array('d',[])
    exp_ee_m1 = array('d',[])
    exp_ee_m2 = array('d',[])

    exp_mm_p2 = array('d',[])
    exp_mm_p1 = array('d',[])
    exp_mm_m1 = array('d',[])
    exp_mm_m2 = array('d',[])

    exp_0p1 = array('d',[])
    exp_0p2 = array('d',[])
    exp_0p3 = array('d',[])


    parser=parseOptions()
    (options,args) = parser.parse_args()

    cut=options.CutChain
    perBinStatUnc=options.PerBinStatUnc
    isDyMC=options.DyMC
    isGJets= (not isDyMC)
    observable=options.Observable
    unblind=options.unblind

    zjetsMethod = 'mczjet'
    if( not isDyMC) :
      zjetsMethod = 'gjet'

    outdirPre = zjetsMethod+'_'+cut
    if(perBinStatUnc) :
      outdir = outdirPre + '_perBinStatUnc'


    mH=[750,800,1200,2000]
    for m in mH:

      scale = 24.7549/1000
      mass.append(float(m))
      zeros.append(0.0)
      sigType = "Graviton2PB_width0"
      outdir = outdirPre + '_' + sigType + '_' + observable

      f = TFile("Datacards/"+outdir+"/higgsCombineTest.Asymptotic.mH"+str(m)+".root","READ")
      t = f.Get("limit")
    
      t.GetEntry(2)
      thisexp = t.limit*scale
      exp.append(thisexp)
      t.GetEntry(0)
      exp_m2.append(thisexp-t.limit*scale)
      t.GetEntry(1)
      exp_m1.append(thisexp-t.limit*scale)
      t.GetEntry(3)
      exp_p1.append(t.limit*scale-thisexp)
      t.GetEntry(4)
      exp_p2.append(t.limit*scale-thisexp)
      t.GetEntry(5)
      obs.append(t.limit*scale)

      fee = TFile("Datacards/"+outdir+"/higgsCombineee_SR.Asymptotic.mH"+str(m)+".root","READ")
      tee = fee.Get("limit")

      tee.GetEntry(2)
      thisexp = tee.limit*scale
      exp_ee.append(thisexp)
      tee.GetEntry(0)
      exp_ee_m2.append(thisexp-tee.limit*scale)
      tee.GetEntry(1)
      exp_ee_m1.append(thisexp-tee.limit*scale)
      tee.GetEntry(3)
      exp_ee_p1.append(tee.limit*scale-thisexp)
      tee.GetEntry(4)
      exp_ee_p2.append(tee.limit*scale-thisexp)
      tee.GetEntry(5)
      obs_ee.append(tee.limit*scale)

      fmm = TFile("Datacards/"+outdir+"/higgsCombinemm_SR.Asymptotic.mH"+str(m)+".root","READ")
      tmm = fmm.Get("limit")

      tmm.GetEntry(2)
      thisexp = tmm.limit*scale
      exp_mm.append(thisexp)
      tmm.GetEntry(0)
      exp_mm_m2.append(thisexp-tmm.limit*scale)
      tmm.GetEntry(1)
      exp_mm_m1.append(thisexp-tmm.limit*scale)
      tmm.GetEntry(3)
      exp_mm_p1.append(tmm.limit*scale-thisexp)
      tmm.GetEntry(4)
      exp_mm_p2.append(tmm.limit*scale-thisexp)
      tmm.GetEntry(5)
      obs_mm.append(tmm.limit*scale)


      sigType = "Graviton2PB_width0p1"
      outdir = outdirPre + '_' + sigType + '_' + observable

      f = TFile("Datacards/"+outdir+"/higgsCombineTest.Asymptotic.mH"+str(m)+".root","READ")
      t = f.Get("limit")

      t.GetEntry(2)
      thisexp = t.limit*scale
      exp_0p1.append(thisexp)

      sigType = "Graviton2PB_width0p2"
      outdir = outdirPre + '_' + sigType + '_' + observable

      f = TFile("Datacards/"+outdir+"/higgsCombineTest.Asymptotic.mH"+str(m)+".root","READ")
      t = f.Get("limit")

      t.GetEntry(2)
      thisexp = t.limit*scale
      exp_0p2.append(thisexp)

      sigType = "Graviton2PB_width0p3"
      outdir = outdirPre + '_' + sigType + '_' + observable

      f = TFile("Datacards/"+outdir+"/higgsCombineTest.Asymptotic.mH"+str(m)+".root","READ")
      t = f.Get("limit")

      t.GetEntry(2)
      thisexp = t.limit*scale
      exp_0p3.append(thisexp)


    print 'mass',mass
    print 'exp',exp
    print 'obs',obs
 
    v_mass = TVectorD(len(mass),mass)
    v_zeros = TVectorD(len(zeros),zeros)
    v_obs = TVectorD(len(obs),obs)
    v_exp = TVectorD(len(exp),exp)
    v_exp_p2 = TVectorD(len(exp_p2),exp_p2)
    v_exp_p1 = TVectorD(len(exp_p1),exp_p1)
    v_exp_m1 = TVectorD(len(exp_m1),exp_m1)
    v_exp_m2 = TVectorD(len(exp_m2),exp_m2)

    v_exp_mm = TVectorD(len(exp_mm),exp_mm)
    v_exp_ee = TVectorD(len(exp_ee),exp_ee)
    v_obs_mm = TVectorD(len(obs_mm),obs_mm)
    v_obs_ee = TVectorD(len(obs_ee),obs_ee)

    v_exp_ee_p2 = TVectorD(len(exp_ee_p2),exp_ee_p2)
    v_exp_ee_p1 = TVectorD(len(exp_ee_p1),exp_ee_p1)
    v_exp_ee_m1 = TVectorD(len(exp_ee_m1),exp_ee_m1)
    v_exp_ee_m2 = TVectorD(len(exp_ee_m2),exp_ee_m2)

    v_exp_mm_p2 = TVectorD(len(exp_mm_p2),exp_mm_p2)
    v_exp_mm_p1 = TVectorD(len(exp_mm_p1),exp_mm_p1)
    v_exp_mm_m1 = TVectorD(len(exp_mm_m1),exp_mm_m1)
    v_exp_mm_m2 = TVectorD(len(exp_mm_m2),exp_mm_m2)

    v_exp_0p1 = TVectorD(len(exp_0p1),exp_0p1)
    v_exp_0p2 = TVectorD(len(exp_0p2),exp_0p2)
    v_exp_0p3 = TVectorD(len(exp_0p3),exp_0p3)


    c = TCanvas("c","c",800, 800)
    c.SetLogy()
    c.SetGridx()
    c.SetGridy()

    c.SetRightMargin(0.06)
    c.SetLeftMargin(0.2)

    dummy = TH1D("dummy","dummy", 1, 750,2000)
    dummy.SetBinContent(1,0.0)
    dummy.GetXaxis().SetTitle('m_{X} [GeV]')   
    dummy.GetYaxis().SetTitle('95% C.L. limit on #sigma(X#rightarrowZZ) [pb]')   
    dummy.SetLineColor(0)
    dummy.SetLineWidth(0)
    dummy.SetFillColor(0)
    dummy.SetMinimum(0.001)
    dummy.SetMaximum(10.0)
    dummy.GetXaxis().SetMoreLogLabels(kTRUE)
    dummy.GetXaxis().SetNoExponent(kTRUE)
    dummy.Draw()

    gr_exp2 = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_exp_m2,v_exp_p2)
    gr_exp2.SetLineColor(kYellow)
    gr_exp2.SetFillColor(kYellow)
    gr_exp2.Draw("e3same")

    gr_exp1 = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_exp_m1,v_exp_p1)
    gr_exp1.SetLineColor(kGreen)
    gr_exp1.SetFillColor(kGreen)
    gr_exp1.Draw("e3same")

    gr_exp = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_exp.SetLineColor(1)
    gr_exp.SetLineWidth(2)
    gr_exp.SetLineStyle(2)
    gr_exp.Draw("csame")

    gr_obs = TGraphAsymmErrors(v_mass,v_obs,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_obs.SetLineColor(1)
    gr_obs.SetLineWidth(3)
    gr_obs.SetMarkerStyle(20)
    if unblind: gr_obs.Draw("plsame")

    gr_exp_0p1 = TGraphAsymmErrors(v_mass,v_exp_0p1,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_exp_0p1.SetLineColor(kRed)
    gr_exp_0p1.SetLineWidth(2)
    gr_exp_0p1.SetLineStyle(2)
    gr_exp_0p1.Draw("csame")

    gr_exp_0p2 = TGraphAsymmErrors(v_mass,v_exp_0p2,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_exp_0p2.SetLineColor(kBlue)
    gr_exp_0p2.SetLineWidth(2)
    gr_exp_0p2.SetLineStyle(2)
    gr_exp_0p2.Draw("csame")

    gr_exp_0p3 = TGraphAsymmErrors(v_mass,v_exp_0p3,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_exp_0p3.SetLineColor(kViolet+2)
    gr_exp_0p3.SetLineWidth(2)
    gr_exp_0p3.SetLineStyle(2)
    gr_exp_0p3.Draw("csame")

    latex1 = TLatex()
    latex1.SetNDC()
    latex1.SetTextSize(0.5*c.GetTopMargin())
    latex1.SetTextFont(42)
    latex1.SetTextAlign(31) # align right
    latex1.DrawLatex(0.87, 0.95,"35.87 fb^{-1} (13 TeV)")
    latex2 = TLatex()
    latex2.SetNDC()    
    latex2.SetTextSize(0.7*c.GetTopMargin())
    latex2.SetTextFont(62)
    latex2.SetTextAlign(11) # align right
    latex2.DrawLatex(0.25, 0.85, "CMS")
    latex3 = TLatex()
    latex3.SetNDC()
    latex3.SetTextSize(0.5*c.GetTopMargin())
    latex3.SetTextFont(52)
    latex3.SetTextAlign(11)
    latex3.DrawLatex(0.25, 0.8, "Preliminary")

    _temp = __import__('BulkGZZ2l2nuXsec', globals(), locals(), ['BulkGZZ2l2nuXsec'], -1)
    BulkGZZ2l2nuXsec = _temp.BulkGZZ2l2nuXsec

    sigXsec={}
    index=0
    for k in ['0.5','0.2','0.1']:
      sigXsec[k] = ROOT.TGraph()
      sigXsec[k].SetName("sigXsec_k"+k)

      N = 0
      for mass in BulkGZZ2l2nuXsec[k].keys():
          sigXsec[k].SetPoint(N,mass,BulkGZZ2l2nuXsec[k][mass]*(25.02502502502502))
          N = N +1
      sigXsec[k].Sort()
      sigXsec[k].SetLineWidth(1)
      sigXsec[k].SetLineColor(ROOT.kRed+index)
      sigXsec[k].SetLineStyle(1+index)
      sigXsec[k].Draw("csame")
      index=index+1

    legend = TLegend(.45,.55,.90,.90)
    for k in ['0.5','0.2','0.1']:
        legend.AddEntry(sigXsec[k] , "RS2 Graviton #tilde{k} = "+k, "l")
    if unblind: legend.AddEntry(gr_obs , "Data Observed ", "pl")
    legend.AddEntry(gr_exp , "Expected ggG width = 0 GeV", "l")
    legend.AddEntry(gr_exp_0p1 , "Expected ggG width = 0.1#timesm_{X}", "l")
    legend.AddEntry(gr_exp_0p2 , "Expected ggG width = 0.2#timesm_{X}", "l")
    legend.AddEntry(gr_exp_0p3 , "Expected ggG width = 0.3#timesm_{X}", "l")
    legend.AddEntry(gr_exp1 , "#pm 1#sigma ", "f")
    legend.AddEntry(gr_exp2 , "#pm 2#sigma ", "f")
    legend.SetShadowColor(0)
    legend.SetFillColor(0)
    legend.SetLineColor(0)            
    legend.Draw("same")
                                                            
    gPad.RedrawAxis()

    if unblind: 
        c.SaveAs("xzz2l2nu_limit_13TeV_SR_ggG_unblind.pdf")
        c.SaveAs("xzz2l2nu_limit_13TeV_SR_ggG_unblind.png")
    else:
        c.SaveAs("xzz2l2nu_limit_13TeV_SR_ggG.pdf")
        c.SaveAs("xzz2l2nu_limit_13TeV_SR_ggG.png")

    ## ee mm compatibility
    c2 = TCanvas("c2","c2",800, 800)
    c2.SetLogy()
    c2.SetGridx()
    c2.SetGridy()
    c2.SetRightMargin(0.06)
    c2.SetLeftMargin(0.2)

    dummy.Draw()

    gr_exp2 = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_exp_m2,v_exp_p2)
    gr_exp2.SetLineColor(kYellow)
    gr_exp2.SetFillColor(kYellow)
    gr_exp2.Draw("e3same")

    gr_exp1 = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_exp_m1,v_exp_p1)
    gr_exp1.SetLineColor(kGreen)
    gr_exp1.SetFillColor(kGreen)
    gr_exp1.Draw("e3same")

    gr_exp = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_exp.SetLineColor(1)
    gr_exp.SetLineWidth(2)
    gr_exp.SetLineStyle(2)
    gr_exp.Draw("csame")

    gr_exp_ee = TGraphAsymmErrors(v_mass,v_exp_ee,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_exp_ee.SetLineColor(kRed)
    gr_exp_ee.SetLineWidth(2)
    gr_exp_ee.SetLineStyle(2)
    gr_exp_ee.Draw("csame")

    gr_exp_mm = TGraphAsymmErrors(v_mass,v_exp_mm,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_exp_mm.SetLineColor(kBlue)
    gr_exp_mm.SetLineWidth(2)
    gr_exp_mm.SetLineStyle(2)
    gr_exp_mm.Draw("csame")

    gr_exp2_ee = TGraphAsymmErrors(v_mass,v_exp_ee,v_zeros,v_zeros,v_exp_ee_m2,v_exp_ee_p2)
    gr_exp2_ee.SetLineColor(kYellow)
    gr_exp2_ee.SetFillColor(kYellow)
    #gr_exp2_ee.Draw("e3same")

    gr_exp1_ee = TGraphAsymmErrors(v_mass,v_exp_ee,v_zeros,v_zeros,v_exp_ee_m1,v_exp_ee_p1)
    gr_exp1_ee.SetLineColor(kGreen)
    gr_exp1_ee.SetFillColor(kGreen)
    #gr_exp1_ee.Draw("e3same")

    gr_exp2_mm = TGraphAsymmErrors(v_mass,v_exp_mm,v_zeros,v_zeros,v_exp_mm_m2,v_exp_mm_p2)
    gr_exp2_mm.SetLineColor(kYellow)
    gr_exp2_mm.SetFillColor(kYellow)
    #gr_exp2_mm.Draw("e3same")

    gr_exp1_mm = TGraphAsymmErrors(v_mass,v_exp_mm,v_zeros,v_zeros,v_exp_mm_m1,v_exp_mm_p1)
    gr_exp1_mm.SetLineColor(kGreen)
    gr_exp1_mm.SetFillColor(kGreen)
    #gr_exp1_mm.Draw("e3same")

    gr_obs_ee = TGraphAsymmErrors(v_mass,v_obs_ee,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_obs_ee.SetLineColor(kRed)
    gr_obs_ee.SetMarkerColor(kRed)
    gr_obs_ee.SetLineWidth(3)
    gr_obs_ee.SetMarkerStyle(20)
    #if unblind: gr_obs_ee.Draw("pcsame")

    gr_obs_mm = TGraphAsymmErrors(v_mass,v_obs_mm,v_zeros,v_zeros,v_zeros,v_zeros)
    gr_obs_mm.SetLineColor(kBlue)
    gr_obs_mm.SetMarkerColor(kBlue)
    gr_obs_mm.SetLineWidth(3)
    gr_obs_mm.SetMarkerStyle(20)
    #if unblind: gr_obs_mm.Draw("pcsame")

    if unblind: gr_obs.Draw("plsame")

    legend2 = TLegend(.45,.60,.90,.90)
    if unblind:    legend2.AddEntry(gr_obs , "Data Observed width = 0 GeV ee+#mu#mu", "pl")
    #if unblind:    legend2.AddEntry(gr_obs_ee , "Data Observed ee", "pl")
    #if unblind:    legend2.AddEntry(gr_obs_mm , "Data Observed #mu#mu", "pl")
    legend2.AddEntry(gr_exp , "Expected ggG width = 0 GeV ee+#mu#mu ", "l")
    legend2.AddEntry(gr_exp_ee , "Expected ggG width = 0 GeV ee", "l")
    legend2.AddEntry(gr_exp_mm , "Expected ggG width = 0 GeV #mu#mu", "l")
    legend2.AddEntry(gr_exp1 , "#pm 1#sigma ggG width = 0 GeV ee+#mu#mu", "f")
    legend2.AddEntry(gr_exp2 , "#pm 2#sigma ggG width = 0 GeV ee+#mu#mu", "f")
    legend2.SetShadowColor(0)
    legend2.SetFillColor(0)
    legend2.SetLineColor(0)
    legend2.Draw("same")

    latex1.DrawLatex(0.87, 0.95,"35.87 fb^{-1} (13 TeV)")
    latex2.DrawLatex(0.25, 0.85, "CMS")
    latex3.DrawLatex(0.25, 0.8, "Preliminary")
    gPad.RedrawAxis()

    if unblind:
        c2.SaveAs("xzz2l2nu_limit_13TeV_ee+mm_SR_ggG_unblind.pdf")
        c2.SaveAs("xzz2l2nu_limit_13TeV_ee+mm_SR_ggG_unbiind.png")
    else:
        c2.SaveAs("xzz2l2nu_limit_13TeV_ee+mm_SR_ggG.pdf")
        c2.SaveAs("xzz2l2nu_limit_13TeV_ee+mm_SR_ggG.png")

    ## ee only
    c3 = TCanvas("c3","c3",800, 800)
    c3.SetLogy()
    c3.SetGridx()
    c3.SetGridy()
    c3.SetRightMargin(0.06)
    c3.SetLeftMargin(0.2)

    dummy.Draw()

    gr_exp2_ee.Draw("e3same")
    gr_exp1_ee.Draw("e3same")
    gr_exp.Draw("csame")
    gr_exp_ee.Draw("csame")

    if unblind: gr_obs_ee.Draw("plsame")

    legend3 = TLegend(.45,.60,.90,.90)
    if unblind:    legend3.AddEntry(gr_obs_ee , "Data Observed ee", "pl")
    legend3.AddEntry(gr_exp , "Expected ggG width = 0 GeV ee+#mu#mu", "l")
    legend3.AddEntry(gr_exp_ee , "Expected ggG width = 0 GeV ee", "l")
    legend3.AddEntry(gr_exp1_ee , "#pm 1#sigma ggG width = 0 GeV ee", "f")
    legend3.AddEntry(gr_exp2_ee , "#pm 2#sigma ggG width = 0 GeV ee", "f")
    legend3.SetShadowColor(0)
    legend3.SetFillColor(0)
    legend3.SetLineColor(0)
    legend3.Draw("same")

    latex1.DrawLatex(0.87, 0.95,"35.87 fb^{-1} (13 TeV)")
    latex2.DrawLatex(0.25, 0.85, "CMS")
    latex3.DrawLatex(0.25, 0.8, "Preliminary")
    gPad.RedrawAxis()

    if unblind:
        c3.SaveAs("xzz2l2nu_limit_13TeV_ee_SR_ggG_unblind.pdf")
        c3.SaveAs("xzz2l2nu_limit_13TeV_ee_SR_ggG_unbiind.png")
    else:
        c3.SaveAs("xzz2l2nu_limit_13TeV_ee_SR_ggG.pdf")
        c3.SaveAs("xzz2l2nu_limit_13TeV_ee_SR_ggG.png")

    ## mm only
    c4 = TCanvas("c4","c4",800, 800)
    c4.SetLogy()
    c4.SetGridx()
    c4.SetGridy()
    c4.SetRightMargin(0.06)
    c4.SetLeftMargin(0.2)

    dummy.Draw()

    gr_exp2_mm.Draw("e3same")
    gr_exp1_mm.Draw("e3same")
    gr_exp.Draw("csame")
    gr_exp_mm.Draw("csame")

    if unblind: gr_obs_mm.Draw("plsame")

    legend4 = TLegend(.45,.60,.90,.90)
    if unblind:    legend4.AddEntry(gr_obs_mm , "Data Observed mm", "pl")
    legend4.AddEntry(gr_exp , "Expected ggG width = 0 GeV ee+#mu#mu", "l")
    legend4.AddEntry(gr_exp_mm , "Expected ggG width = 0 GeV ee", "l")
    legend4.AddEntry(gr_exp1_mm , "#pm 1#sigma ggG width = 0 GeV ee", "f")
    legend4.AddEntry(gr_exp2_mm , "#pm 2#sigma ggG width = 0 GeV ee", "f")
    legend4.SetShadowColor(0)
    legend4.SetFillColor(0)
    legend4.SetLineColor(0)
    legend4.Draw("same")

    latex1.DrawLatex(0.87, 0.95,"35.87 fb^{-1} (13 TeV)")
    latex2.DrawLatex(0.25, 0.85, "CMS")
    latex3.DrawLatex(0.25, 0.8, "Preliminary")
    gPad.RedrawAxis()

    if unblind:
        c4.SaveAs("xzz2l2nu_limit_13TeV_mm_SR_ggG_unblind.pdf")
        c4.SaveAs("xzz2l2nu_limit_13TeV_mm_SR_ggG_unbiind.png")
    else:
        c4.SaveAs("xzz2l2nu_limit_13TeV_mm_SR_ggG.pdf")
        c4.SaveAs("xzz2l2nu_limit_13TeV_mm_SR_ggG.png")

def Run():

    parser=parseOptions()

    plotLimit(parser)

if __name__ == "__main__":
    Run()
