##########################################################
##      configuration for XZZ2l2nu 
##########################################################

import CMGTools.XZZ2l2nu.fwlite.Config as cfg
from CMGTools.XZZ2l2nu.fwlite.Config import printComps
from CMGTools.XZZ2l2nu.RootTools import *
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption


#Load all common analyzers
from CMGTools.XZZ2l2nu.analyzers.coreXZZ_cff import *

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.XZZ2l2nu.samples.loadSamples80xSummer16 import *
selectedComponents = mcSamples+dataSamples

triggerFlagsAna.triggerBits ={
    "ISOMU":triggers_1mu_iso,
    "MU":triggers_1mu_noniso,
    "MUv2":triggers_1mu_noniso_v2,
    "MU50":triggers_1mu_noniso_M50,
    "TkMU50":triggers_1mu_noniso_tkM50,
    "ISOELE":triggers_1e,
    "ELE":triggers_1e_noniso,
    "ELEv2":triggers_1e_noniso_v2,
    "ELE115":triggers_1e_noniso_E115,
    "MUMU": triggers_mumu,
    "MUMUNOISO":triggers_mumu_noniso,
    "ELEL": triggers_ee,
    "HT800":triggers_HT800,
    "HT900":triggers_HT900,
    "JJ":triggers_dijet_fat,
    "MET90":triggers_met90_mht90+triggers_metNoMu90_mhtNoMu90,
    "MET120":triggers_metNoMu120_mhtNoMu120
}

#-------- Analyzer
from CMGTools.XZZ2l2nu.analyzers.treeXZZ_cff import *

leptonicVAna.selectMuMuPair = (lambda x: ((x.leg1.pt()>20 or x.leg2.pt()>20)))
leptonicVAna.selectElElPair =(lambda x: x.leg1.pt()>20.0 or x.leg2.pt()>20.0 )
leptonicVAna.selectVBoson = (lambda x: x.mass()>50.0 and x.mass()<180.0)
multiStateAna.selectPairLLNuNu = (lambda x: x.leg1.mass()>50.0 and x.leg1.mass()<180.0)

#-------- SEQUENCE
coreSequence = [
    skimAnalyzer,
    genAna,
    jsonAna,
    triggerAna,
    pileUpAna,
    vertexAna,
    lepAna,
    jetAna,
    metAna,
#    photonAna,
    leptonicVAna,
    multiStateAna,
    eventFlagsAna,
    triggerFlagsAna,
]
    
#sequence = cfg.Sequence(coreSequence)
sequence = cfg.Sequence(coreSequence+[vvSkimmer,multtrg,vvTreeProducer])
#sequence = cfg.Sequence(coreSequence+[vvSkimmer,fullTreeProducer])
 

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    #selectedComponents = dataSamples
    #selectedComponents = mcSamples
    #selectedComponents = [SingleMuon_Run2015D_Promptv4,SingleElectron_Run2015D_Promptv4]
    #selectedComponents = [SingleMuon_Run2015C_25ns_16Dec]
    #selectedComponents = [SingleMuon_Run2016B_PromptReco_v2] 
    #selectedComponents = [SingleMuon_Run2016D_PromptReco_v2] 
    #selectedComponents = [SingleElectron_Run2016D_PromptReco_v2] 
    #selectedComponents = [SingleElectron_Run2016F_PromptReco_v1, SingleMuon_Run2016F_PromptReco_v1] 
    #selectedComponents = [SingleElectron_Run2016E_PromptReco_v2, SingleMuon_Run2016E_PromptReco_v2] 
    #selectedComponents = [SingleElectron_Run2016D_PromptReco_v2, SingleMuon_Run2016D_PromptReco_v2] 
    #selectedComponents = [SingleElectron_Run2016G_PromptReco_v1] 
    #selectedComponents = [SingleElectron_Run2016G_PromptReco_v1, SingleMuon_Run2016G_PromptReco_v1] 
    #selectedComponents = [SingleElectron_Run2016G_PromptReco_v1, SingleMuon_Run2016G_PromptReco_v1,
    #                      SingleElectron_Run2016F_PromptReco_v1, SingleMuon_Run2016F_PromptReco_v1, 
    #                      SingleElectron_Run2016E_PromptReco_v2, SingleMuon_Run2016E_PromptReco_v2,
    #                      SingleElectron_Run2016D_PromptReco_v2, SingleMuon_Run2016D_PromptReco_v2,
    #                     ] 

    #selectedComponents = [SingleElectron_Run2016H_PromptReco_v1, SingleElectron_Run2016H_PromptReco_v2,
    #                      SingleMuon_Run2016H_PromptReco_v1, SingleMuon_Run2016H_PromptReco_v2, 
    #                      SinglePhoton_Run2016H_PromptReco_v1, SinglePhoton_Run2016H_PromptReco_v2, 
    #                      MET_Run2016H_PromptReco_v1, MET_Run2016H_PromptReco_v2 
    #                     ] 

    #selectedComponents = [SingleElectron_Run2016H_PromptReco_v1] 
    #selectedComponents = [SingleMuon_Run2016H_PromptReco_v3] 
    #selectedComponents = [SingleMuon_Run2016G_PromptReco_v1] 
    #selectedComponents = [SingleElectron_Run2016G_23Sep2016] 
    #selectedComponents = [SingleMuon_Run2016G_23Sep2016] 
    #selectedComponents = [MET_Run2016G_PromptReco_v1] 
    #selectedComponents = [SingleMuon_Run2016B_PromptReco_v2,SingleElectron_Run2016B_PromptReco_v2] 
    #selectedComponents = [MuonEG_Run2015D_16Dec] #MuEG
    #selectedComponents = [RSGravToZZToZZinv_narrow_800]
    #selectedComponents = [DYJetsToLL_M50]
    #selectedComponents = [DYJetsToLL_M50_MGMLM_Ext1]
    #selectedComponents = [DY1JetsToLL_M50_MGMLM]
    #selectedComponents = [DY1JetsToLL_M50_MGMLM, DY2JetsToLL_M50_MGMLM, DY3JetsToLL_M50_MGMLM, DY4JetsToLL_M50_MGMLM, DYBJetsToLL_M50_MGMLM]
    #selectedComponents = [BulkGravToZZToZlepZinv_narrow_1600] 
    #selectedComponents = signalSamples
    #selectedComponents = SingleElectron23Sep2016+SingleMuon23Sep2016+MET23Sep2016+MuonEG23Sep2016
    #selectedComponents =[SingleElectron_Run2016H_PromptReco_v2, SingleMuon_Run2016H_PromptReco_v2, MET_Run2016H_PromptReco_v2,MuonEG_Run2016H_PromptReco_v1,MuonEG_Run2016H_PromptReco_v2] 
    #selectedComponents =[SingleElectron_Run2016H_PromptReco_v1, SingleMuon_Run2016H_PromptReco_v1, MET_Run2016H_PromptReco_v1] 
    #selectedComponents = [SingleMuon_Run2016D_23Sep2016, SingleMuon_Run2016G_23Sep2016, SingleMuon_Run2016B_23Sep2016_v2, SingleElectron_Run2016B_23Sep2016_v2]
    #selectedComponents = signalSamples+backgroundSamples
    #selectedComponents = backgroundSamples
    #selectedComponents = SingleMuon+SingleElectron
    #selectedComponents = [TTTo2L2Nu]
    #selectedComponents = [ZZTo2L2Nu]
    #selectedComponents = [BulkGravToZZ_narrow_800]
    #selectedComponents = [BulkGravToZZToZlepZhad_narrow_800]
#    selectedComponents = SingleElectron23Sep2016
    selectedComponents = SingleMuon23Sep2016
#    selectedComponents = [SingleMuon_Run2016C_23Sep2016]
#    selectedComponents = [SingleMuon_Run2016D_23Sep2016]
#    selectedComponents = [SingleMuon_Run2016E_23Sep2016]
#    selectedComponents = [SingleMuon_Run2016F_23Sep2016]
#    selectedComponents = [SingleMuon_Run2016G_23Sep2016]
#    selectedComponents = [SingleMuon_Run2016H_PromptReco_v1,
#            SingleMuon_Run2016H_PromptReco_v2,
#            SingleMuon_Run2016H_PromptReco_v3,
#            ]
    for c in selectedComponents:
#        c.files = c.files[:2]
        #c.splitFactor = (len(c.files)/10 if len(c.files)>10 else 1)
        c.splitFactor = (len(c.files))
#        c.splitFactor = 1
        #c.triggers=triggers_1mu_noniso
        #c.triggers=triggers_1e_noniso


## output histogram
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='vvTreeProducer/tree.root',
    option='recreate'
    )
outputService.append(output_service)

from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
preprocessor = CmsswPreprocessor("pogRecipes.py")

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],
                     preprocessor=preprocessor, #this would run cmsRun before running Heppy
                     events_class = event_class)

# and the following runs the process directly if running as with python filename.py  
if __name__ == '__main__':
    from PhysicsTools.HeppyCore.framework.looper import Looper
    looper = Looper( 'Loop', config, nPrint = 5,nEvents=300)
    looper.loop()
    looper.write()




