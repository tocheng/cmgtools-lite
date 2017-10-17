import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

#JSON
json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PromptReco/Cert_294927-302663_13TeV_PromptReco_Collisions17_JSON.txt'
run_range = (294927,302663)

### ----------------------------- Run2017B PromptReco v1 ----------------------------------------

SingleElectron_Run2017B_PromptReco_v1 = kreator.makeDataComponent("SingleElectron_Run2017B_PromptReco_v1", "/SingleElectron/Run2017B-PromptReco-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2017B_PromptReco_v1     = kreator.makeDataComponent("SingleMuon_Run2017B_PromptReco_v1"    , "/SingleMuon/Run2017B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2017B_PromptReco_v1   = kreator.makeDataComponent("SinglePhoton_Run2017B_PromptReco_v1"  , "/SinglePhoton/Run2017B-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2017B_PromptReco_v1         = kreator.makeDataComponent("MuonEG_Run2017B_PromptReco_v1"        , "/MuonEG/Run2017B-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=True,jsonFilter=False)

### ----------------------------- Run2017B PromptReco v2 ----------------------------------------

SingleElectron_Run2017B_PromptReco_v2 = kreator.makeDataComponent("SingleElectron_Run2017B_PromptReco_v2", "/SingleElectron/Run2017B-PromptReco-v2/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2017B_PromptReco_v2     = kreator.makeDataComponent("SingleMuon_Run2017B_PromptReco_v2"    , "/SingleMuon/Run2017B-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2017B_PromptReco_v2   = kreator.makeDataComponent("SinglePhoton_Run2017B_PromptReco_v2"  , "/SinglePhoton/Run2017B-PromptReco-v2/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2017B_PromptReco_v2         = kreator.makeDataComponent("MuonEG_Run2017B_PromptReco_v2"        , "/MuonEG/Run2017B-PromptReco-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=True,jsonFilter=True)

### ----------------------------- Run2017C PromptReco v1 ----------------------------------------

SingleElectron_Run2017C_PromptReco_v1 = kreator.makeDataComponent("SingleElectron_Run2017C_PromptReco_v1", "/SingleElectron/Run2017C-PromptReco-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2017C_PromptReco_v1     = kreator.makeDataComponent("SingleMuon_Run2017C_PromptReco_v1"    , "/SingleMuon/Run2017C-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2017C_PromptReco_v1   = kreator.makeDataComponent("SinglePhoton_Run2017C_PromptReco_v1"  , "/SinglePhoton/Run2017C-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2017C_PromptReco_v1         = kreator.makeDataComponent("MuonEG_Run2017C_PromptReco_v1"        , "/MuonEG/Run2017C-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=True,jsonFilter=False)

### ----------------------------- Run2017C PromptReco v2 ----------------------------------------

SingleElectron_Run2017C_PromptReco_v2 = kreator.makeDataComponent("SingleElectron_Run2017C_PromptReco_v2", "/SingleElectron/Run2017C-PromptReco-v2/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2017C_PromptReco_v2     = kreator.makeDataComponent("SingleMuon_Run2017C_PromptReco_v2"    , "/SingleMuon/Run2017C-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2017C_PromptReco_v2   = kreator.makeDataComponent("SinglePhoton_Run2017C_PromptReco_v2"  , "/SinglePhoton/Run2017C-PromptReco-v2/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2017C_PromptReco_v2         = kreator.makeDataComponent("MuonEG_Run2017C_PromptReco_v2"        , "/MuonEG/Run2017C-PromptReco-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=True,jsonFilter=False)

### ----------------------------- Run2017C PromptReco v3 ----------------------------------------

SingleElectron_Run2017C_PromptReco_v3 = kreator.makeDataComponent("SingleElectron_Run2017C_PromptReco_v3", "/SingleElectron/Run2017C-PromptReco-v3/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2017C_PromptReco_v3     = kreator.makeDataComponent("SingleMuon_Run2017C_PromptReco_v3"    , "/SingleMuon/Run2017C-PromptReco-v3/MINIAOD"    , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2017C_PromptReco_v3   = kreator.makeDataComponent("SinglePhoton_Run2017C_PromptReco_v3"  , "/SinglePhoton/Run2017C-PromptReco-v3/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2017C_PromptReco_v3         = kreator.makeDataComponent("MuonEG_Run2017C_PromptReco_v3"        , "/MuonEG/Run2017C-PromptReco-v3/MINIAOD"        , "CMS", ".*root", json, useAAA=True,jsonFilter=False)

### ----------------------------- Run2017D PromptReco v1 ----------------------------------------

SingleElectron_Run2017D_PromptReco_v1 = kreator.makeDataComponent("SingleElectron_Run2017D_PromptReco_v1", "/SingleElectron/Run2017D-PromptReco-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2017D_PromptReco_v1     = kreator.makeDataComponent("SingleMuon_Run2017D_PromptReco_v1"    , "/SingleMuon/Run2017D-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2017D_PromptReco_v1   = kreator.makeDataComponent("SinglePhoton_Run2017D_PromptReco_v1"  , "/SinglePhoton/Run2017D-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2017D_PromptReco_v1         = kreator.makeDataComponent("MuonEG_Run2017D_PromptReco_v1"        , "/MuonEG/Run2017D-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=True,jsonFilter=False)

### ----------------------------- summary ----------------------------------------

SingleMuon=[SingleMuon_Run2017B_PromptReco_v1,
            SingleMuon_Run2017B_PromptReco_v2,
            SingleMuon_Run2017C_PromptReco_v1,
            SingleMuon_Run2017C_PromptReco_v2,
            SingleMuon_Run2017C_PromptReco_v3,
            SingleMuon_Run2017D_PromptReco_v1
            ]

SingleElectron=[SingleElectron_Run2017B_PromptReco_v1,
                SingleElectron_Run2017B_PromptReco_v2,
                SingleElectron_Run2017C_PromptReco_v1,
                SingleElectron_Run2017C_PromptReco_v2,
                SingleElectron_Run2017C_PromptReco_v3,
                SingleElectron_Run2017D_PromptReco_v1
               ]

SinglePhoton=[SinglePhoton_Run2017B_PromptReco_v1,
              SinglePhoton_Run2017B_PromptReco_v2,
              SinglePhoton_Run2017C_PromptReco_v1,
              SinglePhoton_Run2017C_PromptReco_v2,
              SinglePhoton_Run2017C_PromptReco_v3,
              SinglePhoton_Run2017C_PromptReco_v1
             ]

MuonEG=[MuonEG_Run2017B_PromptReco_v1,
        MuonEG_Run2017B_PromptReco_v2,
        MuonEG_Run2017C_PromptReco_v1,
        MuonEG_Run2017C_PromptReco_v2,
        MuonEG_Run2017C_PromptReco_v3,
        MuonEG_Run2017D_PromptReco_v1
      ]

### ----------------------------- summary ----------------------------------------


