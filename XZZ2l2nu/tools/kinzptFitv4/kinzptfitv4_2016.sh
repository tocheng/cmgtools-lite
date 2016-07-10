#!/bin/sh

#file=DYJetsToLL_M50_ZPt
#file=DYJetsToLL_M50
#file=SingleEMU_Run2016B_PromptReco_v1v2
file=BulkGravToZZToZlepZinv_narrow_1000
#file=SingleElectron_Run2015D_16Dec
#file=SingleMuon_Run2015D_16Dec
# compile
g++  -c JetResolutionObject.cc -o JetResolutionObject.o `root-config --cflags` -I.
g++  -c JetResolution.cc -o JetResolution.o `root-config --cflags` -I.
g++  -c KalmanMuonCalibrator.cc -o KalmanMuonCalibrator.o `root-config --cflags` -I.
 
g++ -c kinzptfitv4_2016.cc -o kinzptfitv4_2016.o `root-config --cflags` -I. 
g++ kinzptfitv4_2016.o JetResolutionObject.o JetResolution.o KalmanMuonCalibrator.o -o kinzptfitv4_2016.exe `root-config --cflags` `root-config --libs` -lMinuit2 -I. -L. 

#inputs
#inputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
#outputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
inputdir=skim80x
outputdir=skim80x
#inputdir=skim2
#outputdir=skim2
mkdir -p ${outputdir}
infile=${inputdir}/${file}.root
outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnly.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepRes1p2HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepRes1p6HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepRes2p0HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrHardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResHardOnlyZpT250.root
  
echo -- Command: ./kinzptfitv4_2016.exe $infile $outfile

./kinzptfitv4_2016.exe $infile $outfile
