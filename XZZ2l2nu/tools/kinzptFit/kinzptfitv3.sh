#!/bin/sh



# compile
g++ -c JetResolutionObject.cc -o JetResolutionObject.o `root-config --cflags` -I.
g++ -c JetResolution.cc -o JetResolution.o `root-config --cflags` -I.
g++ -c kinzptfitv3.cc -o kinzptfitv3.o `root-config --cflags` -I.
g++ kinzptfitv3.o JetResolutionObject.o JetResolution.o  -o kinzptfitv3.exe `root-config --cflags` `root-config --libs` -lMinuit2 -I. -L.

# output tag
outtag="V3OnlyMetShiftSigma"

#inputs
inputdir=/data/XZZ/76X_Ntuple/76X_20160615_Skim
outputdir=/data/XZZ/76X_Ntuple/76X_20160615_METSkim
mkdir -p ${outputdir}



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
DYJetsToLL_M50_ZPt.root
DYJetsToLL_M50_BIG_ZPt.root
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

  ./kinzptfitv3.exe $infile $outfile &> ${outfile}.log &

  njob=$(( njob + 1 ))
  if [ "$njob" -eq "6" ]; then
    wait
    njob="0"
  fi

done

