#!/bin/sh

#file=BulkGravToZZToZlepZinv_narrow_1000
file=DYJetsToLL_M50
#file=DYJetsToLL_M50_MGMLM_Ext1
#file=BulkGravToZZToZlepZinv_narrow_1000
# compile
g++ -c JetResolutionObject.cc -o JetResolutionObject.o `root-config --cflags` -I.
g++ -c JetResolution.cc -o JetResolution.o `root-config --cflags` -I. 
g++ -c kinzptfitv3.cc -o kinzptfitv3.o `root-config --cflags` -I. 
g++ kinzptfitv3.o JetResolutionObject.o JetResolution.o  -o kinzptfitv3.exe `root-config --cflags` `root-config --libs` -lMinuit2 -I. -L. 

#inputs
#inputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
#outputdir=/data/XZZ/76X_Ntuple/76X_JEC_Skim
inputdir=skim
outputdir=skim
mkdir -p ${outputdir}
infile=${inputdir}/${file}.root
#outfile=${outputdir}/${file}_V3OnlyJetsCorrBackBigLessConstr.root
#outfile=${outputdir}/${file}_V3OnlyMetShift.root
#outfile=${outputdir}/${file}_V3OnlyMetShiftSigma.root
#outfile=${outputdir}/${file}_V3MetShiftSigmaBackJet.root
#outfile=${outputdir}/${file}_V3MetShiftSigmaBackJetBigSigma.root
#outfile=${outputdir}/${file}_V3MetShiftSigmaJetBackBigBigSigma.root
outfile=${outputdir}/${file}_V3MetShiftSigmaJetDefaultBigSigma.root
#outfile=${outputdir}/${file}_V3MetShiftSigmaJetDefaultSmallVariationBigSigma.root
  
echo -- Command: ./kinzptfitv3.exe $infile $outfile

./kinzptfitv3.exe $infile $outfile
