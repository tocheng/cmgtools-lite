#!/bin/sh


#file=DYJetsToLL_M50_MGMLM_Ext1
file=DYJetsToLL_M50
# compile
g++ -c JetResolutionObject.cc -o JetResolutionObject.o `root-config --cflags` -I.
g++ -c JetResolution.cc -o JetResolution.o `root-config --cflags` -I. 
g++ -c kinzptfit.cc -o kinzptfit.o `root-config --cflags` -I. 
g++ kinzptfit.o JetResolutionObject.o JetResolution.o  -o kinzptfit.exe `root-config --cflags` `root-config --libs` -lMinuit2 -I. -L. 

#inputs
inputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
outputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
mkdir -p ${outputdir}
infile=${inputdir}/${file}.root
outfile=${outputdir}/${file}_JEC.root
  
echo -- Command: ./kinzptfit.exe $infile $outfile

./kinzptfit.exe $infile $outfile

