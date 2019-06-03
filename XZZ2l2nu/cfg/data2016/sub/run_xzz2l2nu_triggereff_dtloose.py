##########################################################
##      configuration for XZZ2l2nu 
##########################################################

import CMGTools.XZZ2l2nu.fwlite.Config as cfg
from CMGTools.XZZ2l2nu.fwlite.Config import printComps
from CMGTools.XZZ2l2nu.RootTools import *
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption


#Load all common analyzers
from CMGTools.XZZ2l2nu.analyzers.coreXZZ_cff import *
from CMGTools.XZZ2l2nu.analyzers.XZZTrgEff import *

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.XZZ2l2nu.samples.loadSamples92x import *
selectedComponents = dataSamples

triggerFlagsAna.triggerBits ={
    "ISOMU":triggers_1mu_iso,
    "MU":triggers_1mu_noniso,
    "ISOELE":triggers_1e,
    "ELE":triggers_1e_noniso,
}

#-------- Analyzer
from CMGTools.XZZ2l2nu.analyzers.treeXZZ_cff import *
leptonicVAna.selectMuMuPair = (lambda x: ((x.leg1.pt()>40 or x.leg2.pt()>40)))
leptonicVAna.selectElElPair =(lambda x: x.leg1.pt()>50.0 or x.leg2.pt()>50.0 )
leptonicVAna.selectVBoson = (lambda x: x.mass()>70.0 and x.mass()<110.0)
multiStateAna.selectPairLLNuNu = (lambda x: x.leg1.mass()>70.0 and x.leg1.mass()<110.0)

jetAna.smearJets=True

#-------- SEQUENCE
#sequence = cfg.Sequence(coreSequence+[vvSkimmer,vvTreeProducer])
trgEffAna = cfg.Analyzer(
    XZZTrgEff, name="TriggerEfficiencyAnalyzer",
    eleHLT='HLT_Ele105_CaloIdVT_GsfTrkIdT',
    muHLT='HLT_Mu50'
    )

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

#sequence = cfg.Sequence(coreSequence)
sequence = cfg.Sequence(coreSequence+[vvSkimmer,trgEffAna,vvTreeProducer])


 

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    #selectedComponents = [JetHT_Run2015C_25ns_16Dec,JetHT_Run2015D_16Dec]
    selectedComponents = SingleMuon+SingleElectron
    #selectedComponents = [SingleMuon_Run2015D_Promptv4,SingleElectron_Run2015D_Promptv4]
    #[SingleElectron_Run2015D_Promptv4,SingleElectron_Run2015D_05Oct]
    #selectedComponents = [RSGravToZZToZZinv_narrow_800]
    #selectedComponents = [BulkGravToZZ_narrow_800]
    #selectedComponents = [BulkGravToZZToZlepZhad_narrow_800]
    for c in selectedComponents:
        #c.files = c.files[0]
        c.splitFactor =  (len(c.files)/5 if len(c.files)>5 else 1)
        #c.splitFactor = 1
        c.triggers=[]
        c.vetoTriggers = []
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

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
event_class = Events
if getHeppyOption("nofetch"):
    event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],
                     events_class = event_class)




