#!/usr/bin/env python
import math,sys
import ROOT
import os, string, math, pickle
from CMGTools.XZZ2l2nu.plotting.TreePlotter import TreePlotter
from CMGTools.XZZ2l2nu.plotting.MergedPlotter import MergedPlotter
from CMGTools.XZZ2l2nu.plotting.StackPlotter import StackPlotter

ROOT.gROOT.SetBatch()
tag="test_"
channel='el'
LogY=False
test=True
DrawLeptons=True
doRhoScale=True

if test: DrawLeptons = False

lepsf="trgsf*isosf*idsf*trksf"

if doRhoScale: 
    tag+="RhoWt_"
    lepsf=lepsf+"*(0.602*exp(-0.5*pow((rho-8.890)/6.187,2))+0.829*exp(-0.5*pow((rho-21.404)/10.866,2)))"

outdir='plots'

indir='/home/heli/XZZ/80X_20160825_light_Skim'

lumi=12.9
sepSig=True
doRatio=True
Blind=True
FakeData=False
UseMETFilter=True

puWeight='puWeight68075'

ZJetsZPtWeight=True
DataHLT=True
k=1 # signal scale

elChannel='((abs(llnunu_l1_l1_pdgId)==11||abs(llnunu_l1_l2_pdgId)==11)&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)'
muChannel='((abs(llnunu_l1_l1_pdgId)==13||abs(llnunu_l1_l2_pdgId)==13)&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))'

if not os.path.exists(outdir): os.system('mkdir '+outdir)

tag = tag+puWeight+'_'


if UseMETFilter: tag = tag+'metfilter_'

if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'
if LogY: tag = tag+'log_'

paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.3}".format(float(lumi))+" fb^{-1}"

metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_CSCTightHalo2015Filter&&Flag_eeBadScFilter)'

#cuts_loose='(nllnunu)'
#cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))"
#cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))"
cuts_zmass="(llnunu_l1_mass>50&&llnunu_l1_mass<180)&&!(llnunu_l1_mass>70&&llnunu_l1_mass<110)"
#cuts_zmass="(llnunu_l1_mass>50&&llnunu_l1_mass<180)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_met50="(llnunu_l2_pt>50)"
cuts_met100="(llnunu_l2_pt>100)"
cuts_met200="(llnunu_l2_pt>200)"
#cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
#cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
#cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
#cuts_loose_zll_met100="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
#cuts_loose_zll_met200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"

cuts=cuts_zmass+'&&(llnunu_l1_pt>{0})&&(llnunu_l2_pt>{0})'.format(sys.argv[1])
#cuts=cuts_zmass

if channel=='el': cuts = cuts+'&&'+elChannel
elif channel=='mu': cuts = cuts+'&&'+muChannel
else: cuts = cuts+'&&({0}||{1})'.format(elChannel,muChannel)
if UseMETFilter:
    cuts = '('+cuts+'&&'+metfilter+')'

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

allPlotters = {}

vvPlotters=[]
vvSamples = ['WZTo2L2Q','WZTo3LNu_AMCNLO',
'ZZTo2L2Nu',
'ZZTo2L2Q','ZZTo4L']
for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    vvPlotters[-1].addCorrectionFactor('xsec','xsec')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    allPlotters[sample] = vvPlotters[-1]
VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)

nonresPlotters=[]
nonresSamples = ['muonegtrgsf']
for sample in nonresSamples:
    nonresPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    nonresPlotters[-1].addCorrectionFactor('etrgsf', 'etrgsf')
#    nonresPlotters[-1].addCorrectionFactor('3.5e-5', 'norm')
#    nonresPlotters[-1].addCorrectionFactor('0.473706296376', 'norm')
    nonresPlotters[-1].addCorrectionFactor('1./({0}*1000)'.format(lumi), 'lumi')
    allPlotters[sample] = nonresPlotters[-1]
NONRES = MergedPlotter(nonresPlotters)
NONRES.setFillProperties(1001,ROOT.kOrange)


zjetsPlotters=[]
#zjetsSamples = ['DYJetsToLL_M50_RecoilNoSmooth','DYJetsToLL_M50_MGMLM_Ext1_RecoilNoSmooth']
zjetsSamples = ['DYJetsToLL_M50_RecoilSmooth','DYJetsToLL_M50_MGMLM_Ext1_RecoilSmooth']
#zjetsSamples = ['DYJetsToLL_M50','DYJetsToLL_M50_MGMLM_Ext1']
#zjetsSamples = ['DYJetsToLL_M50_TgEfElFineBin','DYJetsToLL_M50_MGMLM_Ext1_TgEfElFineBin']
#zjetsSamples = ['DYJetsToLL_M50_Resbos','DYJetsToLL_M50_MGMLM_Ext1_Resbos']
#zjetsSamples = ['DYJetsToLL_M50'] # M50
#zjetsSamples = ['DYJetsToLL_M50_RecoilSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_RecoilNoSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_MGMLM_Ext1_RecoilSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_MGMLM_Ext1_RecoilNoSmooth'] # M50
#zjetsSamples = ['DYJetsToLL_M50_MGMLM_Ext1'] # M50
#zjetsSamples = ['DYJetsToLL_M50_NoRecoil'] # M50
for sample in zjetsSamples:
    zjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    #zjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    zjetsPlotters[-1].addCorrectionFactor('(1.05)','norm')
    if ZJetsZPtWeight: zjetsPlotters[-1].addCorrectionFactor('ZPtWeight','ZPtWeight')
    #zjetsPlotters[-1].addCorrectionFactor('PhiStarWeight','PhiStarWeight')
    #zjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    zjetsPlotters[-1].addCorrectionFactor('(1921.8*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118
    #zjetsPlotters[-1].addCorrectionFactor('(1907.0*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_fsrOn_lowstat_inclusive_NNPDF30_nlo_as_0118
    #zjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    zjetsPlotters[-1].addCorrectionFactor('ZJetsGenWeight','genWeight')
    zjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    zjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    allPlotters[sample] = zjetsPlotters[-1]
ZJets = MergedPlotter(zjetsPlotters)
ZJets.setFillProperties(1001,ROOT.kGreen+2)

dataPlotters=[]
dataSamples = [
#'SingleMuon_Run2016B_PromptReco',
#'SingleElectron_Run2016B_PromptReco',
#'SingleMuon_Run2016B_PromptReco_v2',
#'SingleElectron_Run2016B_PromptReco_v2',
#'SingleMuon_Run2016C_PromptReco_v2',
#'SingleElectron_Run2016C_PromptReco_v2',
#'SingleMuon_Run2016D_PromptReco_v2',
#'SingleElectron_Run2016D_PromptReco_v2',
#'SingleEMU_Run2016BCD_PromptReco_killdup', 
#'SingleEMU_Run2016BCD_PromptReco_noRecoil', 
#'SingleEMU_Run2016BCD_PromptReco_newRecoil', 
#'SingleEMU_Run2016BCD_PromptReco_killdup_old', 
'SingleEMU_Run2016BCD_PromptReco', 
]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))

if DataHLT:
    dataPlotters[0].addCorrectionFactor('(HLT_MUv2||HLT_ELEv2)','HLT')
#    dataPlotters[0].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[1].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')
#    dataPlotters[2].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[3].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')
#    dataPlotters[4].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[5].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')
#    dataPlotters[6].addCorrectionFactor('(HLT_MU)','HLT')
#    dataPlotters[7].addCorrectionFactor('(HLT_ELE&&!HLT_MU)','HLT')


Data = MergedPlotter(dataPlotters)




Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
#Stack.addPlotter(WJets, "WJets","W+Jets", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
Stack.addPlotter(NONRES, "NONReso","non reson.", "background")
#Stack.addPlotter(ggZZ, "ggZZ","ggZZ", "background")
Stack.addPlotter(ZJets, "ZJets","ZJets", "background")

Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'
print cuts
if test: 
    Stack.drawStack('1', cuts, str(lumi*1000), 1, 0, 2, titlex = "1", units = "",output=tag,outDir=outdir,separateSignal=sepSig)

Stack.closePSFile()


