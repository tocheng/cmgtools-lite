#!/bin/env python

import ROOT, string
import math
from math import *

from ROOT import *
from array import array

import sys, os, pwd, commands
from subprocess import *
import optparse, shlex, re

from tdrStyle import *
setTDRStyle()

# load signal modules
sys.path.append('./SigInputs')

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

def parseOptions():

    usage = ('usage: %prog [options]\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)

    parser.add_option("--cutChain", dest="CutChain",default='zpt100met50',help="kinematic cuts on zpt and met")
    parser.add_option("--dyMC",action="store_true", dest="DyMC", default=False,help="whether use DY MC to predict Z+jets MT spectrum")
    parser.add_option("--obs", dest="Observable",default='mT',help="template observable")
    parser.add_option("--perBinStatUnc",action="store_true", dest="PerBinStatUnc", default=False,help="whether do per-bin statistical uncertainty")
    #parser.add_option("--sigType", dest='SigType', type='string',default='BulkGrav_narrow',help='signal type')
    parser.add_option("--unblind",action="store_true", dest="unblind", default=False,help="unblind")

    parser.add_option("-l",action="callback",callback=callback_rootargs)
    parser.add_option("-q",action="callback",callback=callback_rootargs)
    parser.add_option("-b",action="callback",callback=callback_rootargs)

    #(option, args) = parser.parse_args()
    return parser 

def plotDataMC(sigType, channel,cat, parser):
    
    pedestal = 1e-10
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

    fs=""
    if(channel=="mm") :
      fs = "mu"
    elif(channel=="ee") :
      fs = "el"

    outdir = zjetsMethod+'_'+cut
    #outdir = outdir + '_bin'+str(bin_index)
    if(perBinStatUnc) :
      outdir = outdir + '_perBinStatUnc'
    outdir = outdir + '_' + sigType + '_' + observable

    #prefix = "GJets_GMCEtaWt_GMCPhPtWt_"+cat+"_puWeightsummer16_muoneg_"+zjetsMethod+"_metfilter_unblind"
    prefix = "ReMiniAOD_GMCPhPtWt_"+cat+"_puWeightsummer16_muoneg_"+zjetsMethod+"_metfilter_unblind"
    inputfileName = prefix + "_" + fs + "_log_1pb"
    inputfile = TFile('Templates/'+inputfileName+".root","READ")

    ## Templates and Systematic Uncertainties
    Shape = {}
    Shape_orig = {}
    Shape_ups = {}
    Shape_dns = {}

    Shape_fit_ws = {}
    Shape_fit = {}

    processes = ['NonReso','VVZReso','ZJets', 'BulkGravToZZToZlepZinv_narrow_600', 'BulkGravToZZToZlepZinv_narrow_1000', 'BulkGravToZZToZlepZinv_narrow_1600']
    processNameinDC = {'NonReso':'nonreso','VVZReso':'vvreso','ZJets':'zjets', 'BulkGravToZZToZlepZinv_narrow_600':'mX600','BulkGravToZZToZlepZinv_narrow_1000':'mX1000','BulkGravToZZToZlepZinv_narrow_1600':'mX1600'}

    systs = ['fidxsec']#,'id','trg','ewk','qcd','JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','Uncluster','Recoil']
    #systsMT = ['JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','Uncluster','Recoil']

    #bins = [0.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850, 900, 1000, 1100, 1250, 1650, 3000.0]
    #bins = [0.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 650.0, 750.0, 850.0, 950, 1050, 1150, 1250, 1650, 3000.0]
    bins = [0.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 600.0, 700.0, 800.0, 900, 1050, 1150, 1250, 1650, 3000.0]
    if(observable=="MET"):
       #bins = [0.0, 50, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0]
       #bins = [0.0, 50, 60, 80, 100.0, 120.0, 140, 160, 180, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0]
       bins = [0.0, 50, 100.0, 150, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 600, 700, 1000, 1500]

    nbins = len(bins)-1

    errorUps = [0.0] *(nbins)
    errorDns = [0.0] *(nbins)

    stack = THStack("stack","stack")    
    added = TH1D('added', 'total bkg',nbins,array('d',bins))

    for process in processes :

       Shape_orig[process] = (inputfile.Get(observable+"_"+process)).Clone()
       Shape[process] = TH1D(process, process, nbins, array('d',bins))
       rebinMerge(Shape[process],Shape_orig[process])  

       Shape[process].SetName(processNameinDC[process])
       Shape[process].SetTitle(processNameinDC[process])

       for i in range(0,Shape[process].GetXaxis().GetNbins()) :

          if(Shape[process].GetBinContent(i+1)<pedestal) :
            Shape[process].SetBinContent(i+1,pedestal)

       for syst in systs :

         if(process=='NonReso') :
           continue
         if(process!='Zjets' and process!='VVZReso') : 
           continue
         if(process!='ZJets' and syst == 'Recoil' ) :
           continue

         Shape_dns[syst+"_"+process] = TH1D(syst+"Up_"+process, process, len(bins)-1,array('d',bins))
         rebinMerge(Shape_dns[syst+"_"+process],inputfile.Get(observable+"_"+syst+"Dn_"+process))
         Shape_ups[syst+"_"+process] = TH1D(syst+"Dn_"+process, process, len(bins)-1,array('d',bins))
         rebinMerge(Shape_ups[syst+"_"+process],inputfile.Get(observable+"_"+syst+"Up_"+process))

         if(syst=="trg" or syst=="id" or syst=="fidxsec") :

           Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+fs+"Down")
           Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+fs+"Up")
           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process])
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process])

         elif(syst in systsMT and processNameinDC[process]=='zjets'):        

           if(syst!='Recoil'):
             Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_phyMET"+syst+"Down")
             Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_phyMET"+syst+"Up")
           else :
             Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_zjets"+syst+"Down")
             Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_zjets"+syst+"Up")

           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process])
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process])

         else :        

           Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+"Down")
           Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+"Up")
           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process])
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process])

         for i in range(0,Shape[process].GetXaxis().GetNbins()) :

            if(Shape[process].GetBinContent(i+1)<=pedestal) :
               Shape_dns[syst+"_"+process].SetBinContent(i+1,pedestal)
            if(Shape[process].GetBinContent(i+1)<=pedestal) :
               Shape_ups[syst+"_"+process].SetBinContent(i+1,pedestal)

         if(process=='VVZReso' or process=='ZJets'):
             for i in range(1,Shape[process].GetXaxis().GetNbins()+1) :
                 max_val = max(Shape_ups[syst+"_"+process].GetBinContent(i),Shape_dns[syst+"_"+process].GetBinContent(i))  
                 min_val = min(Shape_ups[syst+"_"+process].GetBinContent(i),Shape_dns[syst+"_"+process].GetBinContent(i))
                 error_up = fabs(max_val-Shape[process].GetBinContent(i))
                 error_dn = fabs(min_val-Shape[process].GetBinContent(i))
                 errorUps[i-1] = pow(errorUps[i-1]*errorUps[i-1]+error_up*error_up,0.5)
                 errorDns[i-1] = pow(errorDns[i-1]*errorDns[i-1]+error_dn*error_dn,0.5)


       # MC statistical uncertainty
       if(process=='VVZReso' or process=='NonReso' or process=='ZJets'):

         Shape_dns["statUnc_"+process] = Shape[process].Clone()
         Shape_ups["statUnc_"+process] = Shape[process].Clone()

         Shape_dns["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Down")
         Shape_ups["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Up")

         Shape_dns["statUnc_"+process].SetTitle(processNameinDC[process])
         Shape_ups["statUnc_"+process].SetTitle(processNameinDC[process])

         for i in range(1,Shape[process].GetXaxis().GetNbins()+1) :

           dn = max(pedestal,Shape[process].GetBinContent(i)-fabs(0.5*Shape[process].GetBinError(i)))
           up = Shape[process].GetBinContent(i)+0.5*fabs(Shape[process].GetBinError(i))

           if(Shape[process].GetBinContent(i)<=pedestal) :
             dn = 0.5*pedestal
             up = 2*pedestal

           Shape_dns["statUnc_"+process].SetBinContent(i,dn)
           Shape_ups["statUnc_"+process].SetBinContent(i,up)

           error_up = fabs(up-Shape[process].GetBinContent(i))
           error_dn = fabs(dn-Shape[process].GetBinContent(i))
           errorUps[i-1] = pow(errorUps[i-1]*errorUps[i-1]+error_up*error_up,0.5)
           errorDns[i-1] = pow(errorDns[i-1]*errorDns[i-1]+error_dn*error_dn,0.5)

    ## obs data
    data_obs_orig = (inputfile.Get(observable+"_data")).Clone()
    #data_obs = TH1D(process, process, len(bins)-1,array('d',bins))
    #rebinMerge(data_obs,data_obs_orig)

    blindingCut = 300
    if unblind: blindingCut = 100000 
    if(observable=="MET"):
       blindingCut = 200
       if unblind: blindingCut = 100000 # 200
    hmask_data = blindHist(data_obs_orig,blindingCut)
    # total uncertainty from prefit templates in MaxLikelihood
    processes = ['total_background']
    if observable=="MET": 
        inputfileML = TFile("Diagnosis/mlfit_b-only_met.root","READ")
    else: 
        #inputfileML = TFile("Diagnosis/mlfit_b-only.root","READ")
        inputfileML = TFile("Diagnosis/mlfit_obs.root","READ")
  
    for process in processes :
       print process
       Shape_fit_ws[process] = (inputfileML.Get('shapes_prefit/'+channel+'_'+cat+'/'+process)).Clone()
       #Shape_fit_ws[process] = (inputfileML.Get('shapes_fit_b/'+channel+'_'+cat+'/'+process)).Clone()

    errorUpsFull = [0.0] *(nbins)
    errorDnsFull = [0.0] *(nbins)

    # ratio bins
    r_bins = [0,0.001]+bins[1:-2] + [bins[-1]-0.001,bins[-1]] # extend lower and higher bins
    r_nbins = len(r_bins)-1
   
    print r_bins

    r_full = TH1D('r_full', '',r_nbins,array('d',r_bins))
    r = TH1D('r', '',r_nbins,array('d',r_bins))

    Total_bkg = Shape_fit_ws['total_background']

    for i in range(1,Total_bkg.GetXaxis().GetNbins()+1) :

        errorUpsFull[i-1] = Total_bkg.GetBinError(i)/Total_bkg.GetBinContent(i)
        errorDnsFull[i-1] = Total_bkg.GetBinError(i)/Total_bkg.GetBinContent(i)

        errorUps[i-1] = errorUps[i-1]/Total_bkg.GetBinContent(i)
        errorDns[i-1] = errorDns[i-1]/Total_bkg.GetBinContent(i)

        print i
        r_full.SetBinContent(i+1,1)
        r_full.SetBinError(i+1,max(errorUpsFull[i-1],errorDnsFull[i-1]))
        print max(errorUpsFull[i-1],errorDnsFull[i-1])
        r.SetBinContent(i+1,1)
        r.SetBinError(i+1,max(errorUps[i-1],errorDns[i-1]))
        print max(errorUps[i-1],errorDns[i-1])
        # fill lowest and highest bin
        if i==1 : 
            r_full.SetBinContent(1,1)
            r_full.SetBinError(1,max(errorUpsFull[i-1],errorDnsFull[i-1]))
            r.SetBinContent(1,1)
            r.SetBinError(1,max(errorUps[i-1],errorDns[i-1]))
        if i==nbins :
            r_full.SetBinContent(r_nbins,1)
            r_full.SetBinError(r_nbins,max(errorUpsFull[i-1],errorDnsFull[i-1]))
            r.SetBinContent(r_nbins,1)
            r.SetBinError(r_nbins,max(errorUps[i-1],errorDns[i-1]))            


    #
    r.SetMarkerColor(kCyan+3)
    r.SetLineColor(kCyan+3)
    r.SetFillColor(kCyan+3)
    r.SetFillStyle(3001)

    r_full.SetMarkerColor(kGreen+3)
    r_full.SetLineColor(kGreen+3)
    r_full.SetFillColor(kGreen+3)
    r_full.SetFillStyle(3001)

    #plotting data/MC
    c1 = TCanvas("c1","c1", 800, 800)
    c1.SetLogy()
    c1.SetBottomMargin(0.3)
    c1.SetRightMargin(0.03);

    dummy = TH1D("dummy","dummy", 1, 0,bins[nbins])
    #dummy = TH1D("dummy","dummy", 1, 0, 2000)

    dummy.SetBinContent(1,0.0)
    dummy.GetXaxis().SetTitle(observable+' (GeV)')
    if observable=="MET": dummy.GetYaxis().SetTitle('Events / 50 GeV')
    else: dummy.GetYaxis().SetTitle('Events / 50 GeV') 
    dummy.SetLineColor(0)
    dummy.SetLineWidth(0)
    dummy.SetFillColor(0)
    dummy.SetMinimum(0.1)
    dummy.SetMaximum(1000000.0)
    dummy.GetXaxis().SetMoreLogLabels(kTRUE)
    dummy.GetXaxis().SetNoExponent(kTRUE)
    dummy.GetXaxis().SetTitleSize(0)
    dummy.GetXaxis().SetLabelSize(0)
    dummy.Draw()

    stack.Add(Shape_orig["NonReso"])
    stack.Add(Shape_orig["VVZReso"])
    stack.Add(Shape_orig["ZJets"])
    stack.Draw("histsame")

    stack.GetXaxis().SetMoreLogLabels(kTRUE)
    stack.GetXaxis().SetNoExponent(kTRUE)
    stack.GetXaxis().SetTitleSize(0)
    stack.GetXaxis().SetLabelSize(0)

    data_obs_orig.SetMarkerStyle(20)
    data_obs_orig.SetMarkerSize(1)
    data_obs_orig.SetBinErrorOption(TH1.kPoisson)
    data_obs_orig.Draw("ex0psame")

    hmask_data.Draw("HIST,SAME")

    Shape_orig["BulkGravToZZToZlepZinv_narrow_600"].SetLineColor(ROOT.kRed+0)
    Shape_orig["BulkGravToZZToZlepZinv_narrow_1000"].SetLineColor(ROOT.kRed+1)
    #Shape_orig["BulkGravToZZToZlepZinv_narrow_1600"].SetLineColor(ROOT.kRed+0)
    Shape_orig["BulkGravToZZToZlepZinv_narrow_600"].Draw("histsame")
    Shape_orig["BulkGravToZZToZlepZinv_narrow_1000"].Draw("histsame")
    #Shape_orig["BulkGravToZZToZlepZinv_narrow_1600"].Draw("histsame")

    legend = TLegend(.55,.6,.90,.90)
    legend.AddEntry(Shape_orig["ZJets"], 'Z+jets', "f")
    legend.AddEntry(Shape_orig["VVZReso"], 'Reson. backgrounds', "f")
    legend.AddEntry(Shape_orig["NonReso"], 'Non-Reson. backgrounds', "f")
    legend.AddEntry(Shape_orig["BulkGravToZZToZlepZinv_narrow_600"], '1 pb BulkG-600', "f")
    legend.AddEntry(Shape_orig["BulkGravToZZToZlepZinv_narrow_1000"], '1 pb BulkG-1000', "f")
    #legend.AddEntry(Shape_orig["BulkGravToZZToZlepZinv_narrow_1600"], '1 pb BulkG-1600', "f")
    legend.AddEntry(data_obs_orig, 'Data', "p")
    #legend.AddEntry(r, 'Stat. Uncertainty', "f")
    #legend.AddEntry(r_full, 'Full Uncertainty', "f")
    legend.AddEntry(r_full, 'Sys. Uncertainty', "f")
    legend.SetShadowColor(0);
    legend.SetFillColor(0);
    legend.SetLineColor(0);
    legend.Draw("same")

    latex2 = TLatex()
    latex2.SetNDC()
    latex2.SetTextSize(0.6*c1.GetTopMargin())
    latex2.SetTextFont(42)
    latex2.SetTextAlign(31) # align right
    #latex2.DrawLatex(0.92, 0.94," 35.87 fb^{-1} (13 TeV)")
    latex2.DrawLatex(0.92, 0.94,"#sqrt{s} = 13 TeV 2016 L = 35.9 fb^{-1}")
    latex2.SetTextSize(0.4*c1.GetTopMargin())
    latex2.SetTextFont(52)
    latex2.SetTextAlign(11) # align right
    latex2.DrawLatex(0.2, 0.94, "CMS")
    latex2.SetTextSize(0.4*c1.GetTopMargin())
    latex2.SetTextFont(52)
    latex2.SetTextAlign(11)
    latex2.DrawLatex(0.3, 0.94, "Preliminary")

    gPad.RedrawAxis()

    pad = TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
    pad.SetTopMargin(0.70);
    pad.SetRightMargin(0.03);
    pad.SetFillColor(0);
    #pad.SetGridy(1);
    pad.SetFillStyle(0);
    pad.Draw();
    pad.cd(0);

    dummyR = TH1D("dummyR","dummyR", 1, 0,bins[nbins])
    dummyR.SetBinContent(1,0.0)
    dummyR.GetXaxis().SetTitle(observable+' (GeV)')
    dummyR.SetMinimum(0.5)
    dummyR.SetMaximum(2.0)
    dummyR.GetXaxis().SetMoreLogLabels(kTRUE)
    dummyR.GetXaxis().SetNoExponent(kTRUE)
    dummyR.GetYaxis().SetTitleSize(0.04);
    dummyR.GetYaxis().SetTitleOffset(1.8);
    dummyR.GetYaxis().SetTitle("Data/Bkg.");
    dummyR.GetYaxis().CenterTitle();
    dummyR.GetYaxis().SetLabelSize(0.02);
    dummyR.GetXaxis().SetMoreLogLabels(kTRUE)
    dummyR.GetXaxis().SetNoExponent(kTRUE)
    if observable=="mT": dummyR.GetXaxis().SetTitle("M_{T} (GeV)")
    elif observable=="MET": dummyR.GetXaxis().SetTitle("E_{T}^{miss} (GeV)")
    else: dummyR.GetXaxis().SetTitle(observable+"(GeV)")
    dummyR.Draw()

    added_orig=Shape_orig["NonReso"].Clone()
    added_orig.Add(Shape_orig["VVZReso"])
    added_orig.Add(Shape_orig["ZJets"])

    for i in range(1,added_orig.GetXaxis().GetNbins()+1) :
       added_orig.SetBinError(i,0)

    ratio = data_obs_orig.Clone('ratio')
    ratio.Divide(added_orig)
    ratio.SetLineColor(1)
    ratio.SetMarkerColor(1)
    ratio.SetLineStyle(1)
    ratio.Draw("epsame");

    ratio_mask=blindHist(ratio,blindingCut)
    ratio_mask.Draw("histsame")

    r_full.Draw("e3same")
    r.Draw("e3same")

    if unblind:
        c1.SaveAs('Plots/'+inputfileName+'_'+observable+'_unblind.pdf')
        c1.SaveAs('Plots/'+inputfileName+'_'+observable+'_unblind.png')
    else:
        c1.SaveAs('Plots/'+inputfileName+'_'+observable+'.pdf')
        c1.SaveAs('Plots/'+inputfileName+'_'+observable+'.png')


def rebinMerge(hist,hist_orig) :

    Nbins = hist.GetXaxis().GetNbins()
    Nbins_orig = hist_orig.GetXaxis().GetNbins()

    for i in range(1,Nbins+1):

      lower_edge = hist.GetXaxis().GetBinLowEdge(i)
      higher_edege = hist.GetXaxis().GetBinUpEdge(i)

      bincontent = 0.0
      binerror = 0.0

      for j in range(1,Nbins_orig+1):  
     
        bincenter = hist_orig.GetXaxis().GetBinCenter(j)
        if(bincenter>=lower_edge and bincenter<higher_edege) :
          bincontent = bincontent + hist_orig.GetBinContent(j)
          binerror = binerror + hist_orig.GetBinError(j)*hist_orig.GetBinError(j)
        else :
          continue

      binerror = pow(binerror,0.5)

      hist.SetBinContent(i, bincontent)
      hist.SetBinError(i, binerror)

def blindHist(hist,blindingCut) :
    hmask_data = hist.Clone()
    hmask_data.SetName(hist.GetName()+"_mask")
    for ibb in range(hmask_data.GetNbinsX()+1):

        if hmask_data.GetBinCenter(ibb)<=blindingCut:
             hmask_data.SetBinContent(ibb,-100)
             hmask_data.SetBinError(ibb,0)
        else:
             hist.SetBinContent(ibb,-100)
             hist.SetBinError(ibb,0)
             hmask_data.SetBinContent(ibb,1e100)
             hmask_data.SetBinError(ibb,0)

    hmask_data.SetFillStyle(3003)
    hmask_data.SetFillColor(36)
    hmask_data.SetLineStyle(6)
    hmask_data.SetLineColor(16)

    return hmask_data

### Define function for processing of os command
def processCmd(cmd, quiet = 0):
    #print cmd
    #status, output = commands.getstatusoutput(cmd)
    #output = subprocess.check_output(cmd, shell=True)
    output = '\n'
    p = Popen(cmd, shell=True, stdout=PIPE, stderr=STDOUT,bufsize=-1)
    for line in iter(p.stdout.readline, ''):
        output=output+str(line)
        print line,
    p.stdout.close()
    if p.wait() != 0:
        raise RuntimeError("%r failed, exit status: %d" % (cmd, p.returncode))
    #if (status !=0 and not quiet):
    #    print 'Error in processing command:\n   ['+cmd+']'
    #    print 'Output:\n   ['+output+'] \n'
    if (not quiet):
        print 'Output:\n   ['+output+'] \n'
    return output

def Run():
    parser=parseOptions()
    (options,args) = parser.parse_args()

    sigType="BulkGrav_narrow"

    plotDataMC(sigType, "ee","SR", parser)
    plotDataMC(sigType, "mm","SR", parser)

if __name__ == "__main__":
    Run()
