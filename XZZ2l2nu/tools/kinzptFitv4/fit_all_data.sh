#!/bin/sh


# compile
g++  -c JetResolutionObject.cc -o JetResolutionObject.o `root-config --cflags` -I.
g++  -c JetResolution.cc -o JetResolution.o `root-config --cflags` -I.
g++  -c KalmanMuonCalibrator.cc -o KalmanMuonCalibrator.o `root-config --cflags` -I.

g++ -c kinzptfitv4.cc -o kinzptfitv4.o `root-config --cflags` -I.
g++ kinzptfitv4.o JetResolutionObject.o JetResolution.o KalmanMuonCalibrator.o -o kinzptfitv4.exe `root-config --cflags` `root-config --libs` -lMinuit2 -I. -L.

#inputs
inputdir=skim
outputdir=skim2


# output tag
outtag="V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtectV2MetShift"

#mkdir -p ${outputdir}


list="SingleEMU_Run2015_16Dec.root"
#list="
#SingleElectron_Run2015C_25ns_16Dec.root
#SingleElectron_Run2015D_16Dec.root
#SingleMuon_Run2015C_25ns_16Dec.root
#SingleMuon_Run2015D_16Dec.root
#"


njob="0"

#for infile in DYJetsToLL_M50_BIG_ZPt.root ;
for infile in $list ;
do
  infile=$inputdir/$infile ; 
  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\//}"
  outfile="${outfile/.root/_${outtag}.root}"


  echo -- Input file: $infile
  echo -- Output file: $outfile
  echo -- Command:  ./kinzptfitv3.exe $infile $outfile 

  ./kinzptfitv4.exe $infile $outfile &> ${outfile}.log &

  njob=$(( njob + 1 ))
  if [ "$njob" -eq "6" ]; then
    wait
    njob="0"
  fi

done

