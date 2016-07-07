#!/bin/sh

#file=DYJetsToLL_M50_ZPt
#file=DYJetsToLL_M50
file=SingleEMU_Run2015_16Dec
#file=BulkGravToZZToZlepZinv_narrow_1000
#file=SingleElectron_Run2015D_16Dec
#file=SingleMuon_Run2015D_16Dec
# compile
g++  -c JetResolutionObject.cc -o JetResolutionObject.o `root-config --cflags` -I.
g++  -c JetResolution.cc -o JetResolution.o `root-config --cflags` -I.
g++  -c KalmanMuonCalibrator.cc -o KalmanMuonCalibrator.o `root-config --cflags` -I.
 
g++ -c kinzptfitv4.cc -o kinzptfitv4.o `root-config --cflags` -I. 
g++ kinzptfitv4.o JetResolutionObject.o JetResolution.o KalmanMuonCalibrator.o -o kinzptfitv4.exe `root-config --cflags` `root-config --libs` -lMinuit2 -I. -L. 

#inputs
#inputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
#outputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
#inputdir=skim80x
#outputdir=skim80x
inputdir=skim
outputdir=skim2
mkdir -p ${outputdir}
infile=${inputdir}/${file}.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepSig.root
outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtectV2.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtectMetShift.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtect.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnly.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepRes1p2HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepRes1p6HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepRes2p0HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrSel8HardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResPtErrHardOnlyZpT250.root
#outfile=${outputdir}/${file}_V4_doJetsCorrUseLepResHardOnlyZpT250.root
  
echo -- Command: ./kinzptfitv4.exe $infile $outfile

./kinzptfitv4.exe $infile $outfile
