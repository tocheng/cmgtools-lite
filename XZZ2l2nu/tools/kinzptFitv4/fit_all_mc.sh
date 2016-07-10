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


#DYJetsToLL_M50_ZPt.root
#DYJetsToLL_M50_BIG_ZPt.root

list="
BulkGravToZZToZlepZinv_narrow_1000.root
BulkGravToZZToZlepZinv_narrow_1200.root
BulkGravToZZToZlepZinv_narrow_1400.root
BulkGravToZZToZlepZinv_narrow_1600.root
BulkGravToZZToZlepZinv_narrow_1800.root
BulkGravToZZToZlepZinv_narrow_2000.root
BulkGravToZZToZlepZinv_narrow_2500.root
BulkGravToZZToZlepZinv_narrow_3000.root
BulkGravToZZToZlepZinv_narrow_3500.root
BulkGravToZZToZlepZinv_narrow_4000.root
BulkGravToZZToZlepZinv_narrow_4500.root
BulkGravToZZToZlepZinv_narrow_600.root
BulkGravToZZToZlepZinv_narrow_800.root
TTTo2L2Nu.root
TTWJetsToLNu.root
TTZToLLNuNu.root
WJetsToLNu.root
WWTo2L2Nu.root
WWToLNuQQ.root
WZTo1L1Nu2Q.root
WZTo2L2Q.root
WZTo3LNu.root
ZZTo2L2Nu.root
ZZTo2L2Q.root
ZZTo4L.root
"



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

