#####################
# load samples 
#####################

import PhysicsTools.HeppyCore.framework.config as cfg
import os

# Load backgrounds
#from CMGTools.XZZ2l2nu.samples.samples_13TeV_RunIIMiniAODv2 import *
# Load signals
#from CMGTools.XZZ2l2nu.samples.samples_13TeV_signal92X import *
# Load Data 
from CMGTools.XZZ2l2nu.samples.samples_13TeV_DATA2017 import *
# Load triggers
from CMGTools.XZZ2l2nu.samples.triggers_13TeV_2017 import *

'''
# backgrounds
backgroundSamples = [
DYJetsToLL_M50,
DYJetsToLL_M50_reHLT,
WJetsToLNu,
TTTo2L2Nu,
ZZTo2L2Nu,
ZZTo4L,
ZZTo2L2Q,
WWTo2L2Nu,
WWToLNuQQ,
WWToLNuQQ_Ext1,
WZTo1L1Nu2Q,
WZTo2L2Q,
WZTo3LNu,
WZTo3LNu_AMCNLO,
TTZToLLNuNu,
TTWJetsToLNu,
ggZZTo2e2nu,
ggZZTo2mu2nu,
]

extraBackgroundMC = [
# DYJetsToLL_M50_HT100to200,
# DYJetsToLL_M50_HT200to400,
# DYJetsToLL_M50_HT400to600,
# DYJetsToLL_M50_HT600toInf,
WW,
WZ,
ZZ,
TT,
DY1JetsToLL_M50_MGMLM,
DY2JetsToLL_M50_MGMLM,
DY3JetsToLL_M50_MGMLM,
DY4JetsToLL_M50_MGMLM,
DYBJetsToLL_M50_MGMLM,
DYJetsToLL_M50_Ext,
]



MajorGJetsMC=[
WJetsToLNu,
WGToLNuG,
TTGJets,
TToLeptons_tch_powheg,
TBarToLeptons_tch_powheg,
TBar_tWch,
T_tWch,
TGJets,
TGJets_ext,
GJet_Pt_20toInf_DoubleEMEnriched,
GJet_Pt_40toInf_DoubleEMEnriched,
GJet_Pt_20to40_DoubleEMEnriched,
GJets_HT40to100,
GJets_HT100to200,
GJets_HT200to400,
GJets_HT400to600,
GJets_HT600toInf,
ZNuNuGJetsGt130,
ZNuNuGJetsGt40Lt130,
DYJetsToLL_M50_reHLT,
QCD_HT200to300_ext,
QCD_HT300to500,
QCD_HT300to500_ext,
QCD_HT500to700,
QCD_HT500to700_ext,
#QCD_HT700to1000,
QCD_HT700to1000_ext,
QCD_HT1000to1500,
QCD_HT1000to1500_ext,
QCD_HT1500to2000,
QCD_HT1500to2000_ext,
QCD_HT2000toInf,
QCD_HT2000toInf_ext,
QCD_Pt20to30_EMEnriched,
QCD_Pt30to50_EMEnriched,
QCD_Pt50to80_EMEnriched,
QCD_Pt80to120_EMEnriched,
QCD_Pt120to170_EMEnriched,
QCD_Pt170to300_EMEnriched,
QCD_Pt300toInf_EMEnriched,
ZJetsToNuNu_HT100to200,
ZJetsToNuNu_HT100to200_ext,
ZJetsToNuNu_HT200to400,
ZJetsToNuNu_HT200to400_ext,
ZJetsToNuNu_HT400to600,
#ZJetsToNuNu_HT400to600_ext,
#ZJetsToNuNu_HT600toInf,
ZJetsToNuNu_HT600to800,
#ZJetsToNuNu_HT600to800_ext,
ZJetsToNuNu_HT800to1200,
#ZJetsToNuNu_HT800to1200ext,
ZJetsToNuNu_HT1200to2500,
ZJetsToNuNu_HT1200to2500_ext,
ZJetsToNuNu_HT2500toInf,
#ZJetsToNuNu_HT2500toInf_ext,
WJetsToLNu_HT100to200,
WJetsToLNu_HT100to200_ext,
WJetsToLNu_HT200to400,
WJetsToLNu_HT200to400_ext,
WJetsToLNu_HT400to600,
WJetsToLNu_HT400to600_ext,
WJetsToLNu_HT600to800,
WJetsToLNu_HT600to800_ext,
WJetsToLNu_HT800to1200,
WJetsToLNu_HT800to1200_ext,
WJetsToLNu_HT1200to2500,
WJetsToLNu_HT1200to2500_ext,
WJetsToLNu_HT2500toInf,
WJetsToLNu_HT2500toInf_ext,

]
OtherGJetsMC=[
QCD_Pt50to80,
QCD_Pt80to120,
QCD_Pt120to170,
QCD_Pt170to300,
QCD_Pt300to470,
QCD_Pt470to600,
QCD_Pt600to800,
QCD_Pt800to1000,
QCD_Pt1000to1400,
QCD_Pt1400to1800,
QCD_Pt1800to2400,
QCD_Pt2400to3200,
]


GJetsMC=MajorGJetsMC+OtherGJetsMC

# signals
signalSamples = [
BulkGravToZZToZlepZinv_narrow_600,
BulkGravToZZToZlepZinv_narrow_800,
BulkGravToZZToZlepZinv_narrow_1000,
BulkGravToZZToZlepZinv_narrow_1200,
BulkGravToZZToZlepZinv_narrow_1400,
BulkGravToZZToZlepZinv_narrow_1600,
BulkGravToZZToZlepZinv_narrow_1800,
BulkGravToZZToZlepZinv_narrow_2000,
BulkGravToZZToZlepZinv_narrow_2500,
BulkGravToZZToZlepZinv_narrow_3000,
BulkGravToZZToZlepZinv_narrow_3500,
BulkGravToZZToZlepZinv_narrow_4000,
BulkGravToZZToZlepZinv_narrow_4500,
]

BulkGravToZZToZlepZhad = [
BulkGravToZZToZlepZhad_narrow_600,
BulkGravToZZToZlepZhad_narrow_800,
BulkGravToZZToZlepZhad_narrow_1000,
BulkGravToZZToZlepZhad_narrow_1200,
BulkGravToZZToZlepZhad_narrow_1400,
BulkGravToZZToZlepZhad_narrow_1600,
BulkGravToZZToZlepZhad_narrow_1800,
BulkGravToZZToZlepZhad_narrow_2000,
BulkGravToZZToZlepZhad_narrow_2500,
BulkGravToZZToZlepZhad_narrow_3000,
BulkGravToZZToZlepZhad_narrow_3500,
BulkGravToZZToZlepZhad_narrow_4000,
BulkGravToZZToZlepZhad_narrow_4500,
]

# MC samples
otherMcSamples = BulkGravToZZToZlepZhad  
mcSamples = signalSamples + backgroundSamples + jercRefMC + extraBackgroundMC + GJetsMC
#mcSamples = backgroundSamples

'''

# data
'''
SingleMuon=[SingleMuon_Run2017B_PromptReco_v1,
            SingleMuon_Run2017B_PromptReco_v2,
            SingleMuon_Run2017B_PromptReco_v1

            ]
'''

for s in SingleMuon:
    #s.triggers = triggers_1mu_noniso
    s.triggers = [] 
    s.vetoTriggers = []
for s in SingleElectron:
    #s.triggers = triggers_1e_noniso
    s.triggers = [] 
    #s.vetoTriggers = triggers_1mu_noniso
    s.vetoTriggers = []
for s in SinglePhoton:
    #s.trigers = triggers_all_photons
    s.trigers = []
    s.vetoTriggers = []

for s in MuonEG:
    s.trigers = []
    s.vetoTriggers = []

dataSamples=SingleMuon+SingleElectron
otherDataSamples=MuonEG

allDataSamples=dataSamples+otherDataSamples

# JSON
json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PromptReco/Cert_294927-302663_13TeV_PromptReco_Collisions17_JSON.txt'
run_range = (294927,302663)

jsonFile = json

from CMGTools.XZZ2l2nu.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/XZZ2l2nu/data"

'''
for comp in mcSamples+otherMcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250
    comp.puFileMC=dataDir+"/pileup_MC_80x_271036-276811_68075.root"
    comp.puFileData=dataDir+"/pileup_DATA_80x_271036-276811_68075.root"
    comp.eSFinput=dataDir+"/CutBasedID_LooseWP_76X_18Feb.txt_SF2D.root"
    comp.efficiency = eff2012
    #comp.triggers=triggers_1mu_noniso+triggers_1e_noniso
    comp.triggers= []
    comp.globalTag = "Summer15_25nsV6_MC"
'''

for comp in allDataSamples:
    comp.splitFactor = 250
    comp.isMC = False
    comp.isData = True
    comp.json = jsonFile
    comp.run_range = run_range
    comp.globalTag = "Summer15_25nsV6_DATA"

