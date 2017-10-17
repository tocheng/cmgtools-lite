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
from CMGTools.XZZ2l2nu.samples.loadSamples92X import *
selectedComponents = dataSamples

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
    leptonicVAna,
    multiStateAna,
    eventFlagsAna,
    triggerFlagsAna,
]
    
sequence = cfg.Sequence(coreSequence+[vvSkimmer,multtrg,vvTreeProducer])

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    #selectedComponents = dataSamples
    #selectedComponents = [SingleElectron_Run2016G_PromptReco_v1, SingleMuon_Run2016G_PromptReco_v1,
    #                      SingleElectron_Run2016D_PromptReco_v2, SingleMuon_Run2016D_PromptReco_v2,
    #                     ] 
    selectedComponents = [SingleMuon_Run2017D_PromptReco_v1,SingleElectron_Run2017D_PromptReco_v1]

    for c in selectedComponents:
        #c.files = c.files[:4]
        c.splitFactor = (len(c.files)/3+1 if ((len(c.files) % 3)>0) else len(c.files)/3)
        #c.splitFactor = 2
        #c.triggers=triggers_1mu_noniso

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
preprocessor = CmsswPreprocessor("pogRecipesData92X.py")

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
    looper = Looper( 'Loop', config, nPrint = 1,nEvents=-1)
    looper.loop()
    looper.write()
