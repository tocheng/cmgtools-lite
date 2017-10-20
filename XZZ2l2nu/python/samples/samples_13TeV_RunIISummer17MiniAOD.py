import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

# Photon+Jets



# DY HT bins:
#https://twiki.cern.ch/twiki/bin/view/CMS/SummaryTable1G25ns#DY_Z

# cross-section:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV
#https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToGenXSecAnalyzer
# DY inclusive, NLO RunIISpring16MiniAODv2 
# 28M, x-sec recalculated using FEWZ using z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118 QCD NNLO, QED NLO, including ISR, no FSR (because xsec reduction due to FSR is coming from the M50 mass cut)
DYJetsToLL_M50 = kreator.makeMCComponent("DYJetsToLL_M50", 
"/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v2/MINIAODSIM", 
"CMS", ".*root", 1921.8*3, useAAA=True) 

# LO* NLO kfactor 1.16261343013
# LO xsec calculated from miniAOD
# NLO/LO = njets NLO calculation / LO from miniAOD
#        = 1921.8*3/4959.0 = 1.16261343013
#DY1JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY1JetsToLL_M50_MGMLM",
#"/DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
#"CMS", ".*root", 1013.0*1.16261343013) 
#DY2JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY2JetsToLL_M50_MGMLM",
#"/DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
#"CMS", ".*root", 334.7*1.16261343013) 
#DY3JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY3JetsToLL_M50_MGMLM",
#"/DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
#"CMS", ".*root", 102.4*1.16261343013) 
#DY4JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY4JetsToLL_M50_MGMLM",
#"/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
#"CMS", ".*root", 54.45*1.16261343013) 
#DYBJetsToLL_M50_MGMLM = kreator.makeMCComponent("DYBJetsToLL_M50_MGMLM",
#"/DYBJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
#"CMS", ".*root", 88.2771*1.16261343013) 


# W+Jets

### DiBosons

# cross section from https://twiki.cern.ch/twiki/bin/view/CMS/SummaryTable1G25ns#Diboson

### top

# previous: 104M: gen seg: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_fragment/HIG-RunIIWinter15wmLHE-00518/0 
#TTTo2L2Nu = kreator.makeMCComponent("TTTo2L2Nu", 
#"/TTTo2L2Nu_13TeV-powheg/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM", 
#"CMS", ".*root", 831.76*((3*0.108)**2) ) # 104M
# noSC: 9.8M gen seg: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_fragment/TOP-RunIISummer15wmLHEGS-00049/0 

### gamma+jets
### GJets Xsec: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Gamma_jets

### ggZZ

# QCD HT bins (cross sections from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#QCD

#QCD_Pt10to15     = kreator.makeMCComponent("QCD_Pt10to15"     , 
#"/QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM"     , 
#"CMS" , ".*root" , 5887580000)

# qcd emenr
#QCD_Pt15to20_EMEnriched   = kreator.makeMCComponent("QCD_Pt15to20_EMEnriched"  ,"/QCD_Pt-15to20_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM"  , "CMS", ".*root", 1273000000*0.0002)

#TToLeptons_tch_powheg = kreator.makeMCComponent("TToLeptons_tch_powheg", 
#"/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM", 
#"CMS", ".*root", (136.02)*0.108*3)
#TBarToLeptons_tch_powheg = kreator.makeMCComponent("TBarToLeptons_tch_powheg", 
#"/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM", 
#"CMS", ".*root", 80.95*0.108*3)


### Zinv
#280.47*1.23+78.36*1.23+10.94*1.23+1.474*1.23+0.3586*1.23+0.008203*1.23

### W+jets
# xsec from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# 1345*1.21+359.7*1.21+48.91*1.21+12.05*1.21+5.501*1.21+1.329*1.21+0.03216*1.21

