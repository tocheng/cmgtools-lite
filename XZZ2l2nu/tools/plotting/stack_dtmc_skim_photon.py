#!/usr/bin/env python

import optparse
import ROOT
import os,sys, string, math, pickle
from CMGTools.XZZ2l2nu.plotting.TreePlotter import TreePlotter
from CMGTools.XZZ2l2nu.plotting.MergedPlotter import MergedPlotter
from CMGTools.XZZ2l2nu.plotting.StackPlotter import StackPlotter

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

parser = optparse.OptionParser()
parser.add_option("-t","--tag",dest="tag",default='DataB2G_ICHEPcfg_',help="")
parser.add_option("--cutChain",dest="cutChain",default='tight',help="")
parser.add_option("--LogY",action="store_true", dest="LogY", default=False, help="")
parser.add_option("--Blind",action="store_true", dest="Blind", default=False,help="")
parser.add_option("--test",action="store_true", dest="test", default=False,help="")
parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)





(options,args) = parser.parse_args()


tag=options.tag
cutChain=options.cutChain

LogY=options.LogY
test=options.test
doRhoScale=True
doVtxScale=False
doEtaScale=True

scale='(1)'

if doRhoScale: 
    tag+="RhoWt_"
    scale=scale+"*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))" # b2h rereco 36.1 fb-1

if doVtxScale:
    tag+="VtxWt_"
    scale=scale+"*(0.807+0.007*nVert+-3.689e-05*nVert*nVert+6.730e-04*exp(2.500e-01*nVert))" # b2h rereco 33.59fb-1

if doEtaScale:
    tag+="EtaWt_"
    scale=scale+"*(0.87*TMath::Gaus(llnunu_l1_eta,0.65,0.56)+0.87*TMath::Gaus(llnunu_l1_eta,-0.65,0.56)+0.65*TMath::Gaus(llnunu_l1_eta,1.90,0.25)+0.65*TMath::Gaus(llnunu_l1_eta,-1.90,0.25))"

outdir='plots_ph_36p22'

indir='/home/heli/XZZ/80X_20161029_GJets_light_Skim'
lumi=36.46
sepSig=True
doRatio=True
Blind=options.Blind
puWeight='puWeightmoriondMC'

if not os.path.exists(outdir): os.system('mkdir -p '+outdir)

tag = tag+cutChain+'_'
tag = tag+puWeight+'_'




if not Blind: tag = tag+'unblind_'

if LogY: tag = tag+'log_'


paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.4}".format(float(lumi))+" fb^{-1}"


cuts_loose='(1)'
cuts_tight='llnunu_l1_pt>50'

if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_tight
else : cuts=cuts_loose

cuts = '('+cuts+')'


ROOT.gROOT.ProcessLine('.x tdrstyle.C') 


zllPlotters=[]
zllSamples = ['DYJetsToLL_M50_reHLT']

for sample in zllSamples:
    zllPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    zllPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    zllPlotters[-1].addCorrectionFactor('xsec','xsec')
    zllPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    zllPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    zllPlotters[-1].addCorrectionFactor(scale,'scale')

ZLL = MergedPlotter(zllPlotters)
ZLL.setFillProperties(1001,ROOT.kOrange)


znnPlotters=[]
znnSamples = [
'ZJetsToNuNu_HT100to200_BIG',
'ZJetsToNuNu_HT200to400_BIG',
'ZJetsToNuNu_HT400to600_BIG',
'ZJetsToNuNu_HT600to800_BIG',
'ZJetsToNuNu_HT800t1200_BIG',
'ZJetsToNuNu_HT1200to2500_BIG',
'ZJetsToNuNu_HT2500toInf_BIG',
]

for sample in znnSamples:
    znnPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    znnPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    znnPlotters[-1].addCorrectionFactor('xsec','xsec')
    znnPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    znnPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    znnPlotters[-1].addCorrectionFactor(scale,'scale')

ZNN = MergedPlotter(znnPlotters)
ZNN.setFillProperties(1001,ROOT.kMagenta)

znngPlotters=[]
znngSamples = [
'ZNuNuGJetsGt40Lt130',
'ZNuNuGJetsGt130',
]

for sample in znngSamples:
    znngPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    znngPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    znngPlotters[-1].addCorrectionFactor('xsec','xsec')
    znngPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    znngPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    znngPlotters[-1].addCorrectionFactor(scale,'scale')

ZNNG = MergedPlotter(znngPlotters)
ZNNG.setFillProperties(1001,ROOT.kMagenta+2)


wPlotters=[]
wSamples = [
'WGToLNuG',
'WJetsToLNu',
]

for sample in wSamples:
    wPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wPlotters[-1].addCorrectionFactor('xsec','xsec')
    wPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wPlotters[-1].addCorrectionFactor(scale,'scale')

W = MergedPlotter(wPlotters)
W.setFillProperties(1001,ROOT.kRed)


wgtolnugPlotters=[]
wgtolnugSamples = [
'WGToLNuG',
]

for sample in wgtolnugSamples:
    wgtolnugPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wgtolnugPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wgtolnugPlotters[-1].addCorrectionFactor('xsec','xsec')
    wgtolnugPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wgtolnugPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wgtolnugPlotters[-1].addCorrectionFactor(scale,'scale')

WGToLNuG = MergedPlotter(wgtolnugPlotters)
WGToLNuG.setFillProperties(1001,ROOT.kRed-3)

wjetsPlotters=[]
wjetsSamples = [
#'WJetsToLNu',
'WJetsToLNu_HT100to200_BIG',
'WJetsToLNu_HT1200to2500_BIG',
'WJetsToLNu_HT200to400_BIG',
'WJetsToLNu_HT2500toInf_BIG',
'WJetsToLNu_HT400to600_BIG',
'WJetsToLNu_HT600to800_BIG',
'WJetsToLNu_HT800to1200_BIG',
]

for sample in wjetsSamples:
    wjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    wjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wjetsPlotters[-1].addCorrectionFactor(scale,'scale')

WJETS = MergedPlotter(wjetsPlotters)
WJETS.setFillProperties(1001,ROOT.kYellow)

gjetsPlotters=[]
gjetsSamples = [
#'GJets_HT40to100',
'GJets_HT100to200',
'GJets_HT200to400',
'GJets_HT400to600',
'GJets_HT600toInf',
]

for sample in gjetsSamples:
    gjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    gjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    #gjetsPlotters[-1].addCorrectionFactor('xsec*0.6','xsec')
    gjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    gjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    gjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    gjetsPlotters[-1].addCorrectionFactor(scale,'scale')
#    gjetsPlotters[-1].addCorrectionFactor('GJetsRhoWeight','rhoweight')
#    gjetsPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsWeight')
#    gjetsPlotters[-1].addCorrectionFactor('(172.883)','GJetsNorm') #mu ResBos

GJETS = MergedPlotter(gjetsPlotters)
GJETS.setFillProperties(1001,ROOT.kBlue-6)

'''
gjetsemPlotters=[]
gjetsemSamples = [
'GJet_Pt_20toInf_DoubleEMEnriched',
]

for sample in gjetsemSamples:
    gjetsemPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    gjetsemPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    gjetsemPlotters[-1].addCorrectionFactor('xsec','xsec')
    gjetsemPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    gjetsemPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    gjetsemPlotters[-1].addCorrectionFactor(scale,'scale')

GJETSEM = MergedPlotter(gjetsemPlotters)
GJETSEM.setFillProperties(1001,ROOT.kBlue-3)
'''

qcdPlotters=[]
qcdSamples = [
'QCD_Pt20to30_EMEnriched',
'QCD_Pt30to50_EMEnriched',
'QCD_Pt50to80_EMEnriched',
'QCD_Pt80to120_EMEnriched',
'QCD_Pt120to170_EMEnriched',
'QCD_Pt170to300_EMEnriched',
'QCD_Pt300toInf_EMEnriched',
#'QCD_HT100to200_BIG',
#'QCD_HT200to300_BIG',
#'QCD_HT300to500_BIG',
#'QCD_HT500to700_BIG',
#'QCD_HT700to1000_BIG',
#'QCD_HT1000to1500_BIG',
#'QCD_HT1500to2000_BIG',
#'QCD_HT2000toInf_BIG',
]

for sample in qcdSamples:
    qcdPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    qcdPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    qcdPlotters[-1].addCorrectionFactor('xsec','xsec')
    qcdPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    qcdPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    qcdPlotters[-1].addCorrectionFactor(scale,'scale')
#    qcdPlotters[-1].addCorrectionFactor('GJetsRhoWeight','rhoweight')
#    qcdPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsWeight')
#    qcdPlotters[-1].addCorrectionFactor('(172.883)','GJetsNorm') #mu ResBos


QCD = MergedPlotter(qcdPlotters)
QCD.setFillProperties(1001,ROOT.kGreen+2)

tPlotters=[]
tSamples = [
'TToLeptons_tch_powheg',
'TBarToLeptons_tch_powheg',
'T_tWch',
'TBar_tWch',
'TGJets_BIG',
'TTGJets',
]

for sample in tSamples:
    tPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    tPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    tPlotters[-1].addCorrectionFactor('xsec','xsec')
    tPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    tPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    tPlotters[-1].addCorrectionFactor(scale,'scale')

T = MergedPlotter(tPlotters)
T.setFillProperties(1001,ROOT.kAzure-9)


# all physical met
phymetPlotters = zllPlotters+znnPlotters+znngPlotters+wgtolnugPlotters+wjetsPlotters+tPlotters

#for i in range(len(phymetPlotters)):
#    phymetPlotters[i].addCorrectionFactor('GJetsRhoWeight','rhoweight')
#    phymetPlotters[i].addCorrectionFactor('GJetsZPtWeightMu','GJetsWeight')
#    phymetPlotters[i].addCorrectionFactor('(172.883)','GJetsNorm') #mu ResBos

PhyMET = MergedPlotter(phymetPlotters)
PhyMET.setFillProperties(1001, ROOT.kOrange)



dataPlotters=[]
dataSamples = [
#'SinglePhoton_Run2016B2H_ReReco_36p22fbinv_NoRecoil'
'SinglePhoton_Run2016B2H_ReReco_36p22fbinv_ResBos_Rc36p22'
]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    dataPlotters[-1].addCorrectionFactor('GJetsPreScaleWeight','prescale')
    dataPlotters[-1].addCorrectionFactor('GJetsRhoWeight','rhoweight')
#    dataPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsWeight')
#    dataPlotters[-1].addCorrectionFactor('(172.883)','GJetsNorm') #mu ResBos
#    dataPlotters[-1].setLineProperties(1,ROOT.kBlack,1)
#    dataPlotters[-1].setMarkerProperties(22)


Data = MergedPlotter(dataPlotters)



Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "#gamma Data", "data")
#Stack.addPlotter(Data, "data_obs", "#gamma Data", "signal")
Stack.addPlotter(PhyMET, "PhyMET","Physical MET", "background")
#Stack.addPlotter(ZNN, "ZNN","Z->#nu#nu", "background")
#Stack.addPlotter(ZNNG, "ZNNG","Z#gamma->#nu#nu#gamma", "background")
#Stack.addPlotter(T, "T","Top", "background")
#Stack.addPlotter(ZLL, "ZLL","Z->ll", "background")
##Stack.addPlotter(W, "W","W/WW", "background")
#Stack.addPlotter(WGToLNuG, "WGToLNuG","W#gamma->l#nu#gamma", "background")
#Stack.addPlotter(WJETS, "WJets","W->l#nu", "background")
Stack.addPlotter(GJETS, "GJETS","#gamma+jets", "background")
#Stack.addPlotter(GJETSEM, "GJETSEM","#gamma+jets EmEnrich", "background")
Stack.addPlotter(QCD, "QCD","QCD", "background")

 
Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'

if test: 
    Stack.drawStack('nVert', cuts, str(lumi*1000), 100, 0.0, 100.0, titlex = "N vertices", units = "",output=tag+'nVert',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('rho', cuts, str(lumi*1000), 55, 0.0, 55.0, titlex = "#rho", units = "",output=tag+'rho',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 100, -2.5, 2.5, titlex = "#eta(Z) ", units = "",output=tag+'zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(Z)", units = "",output=tag+'zphi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 50, 0, 1000, titlex = "MET", units = "GeV",output=tag+'met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -500, 500.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -500, 500.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_phi', cuts, str(lumi*1000), 100, -3.2, 3.2, titlex = "#phi(MET)", units = "",output=tag+'metPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_sumEt', cuts, str(lumi*1000), 80, 0.0, 3000.0, titlex = "sumE_{T}", units = "GeV",output=tag+'metSumEt',outDir=outdir,separateSignal=sepSig)


Stack.closePSFile()
Stack.closeROOTFile()

