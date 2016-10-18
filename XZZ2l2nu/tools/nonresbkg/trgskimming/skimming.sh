#!/bin/sh

# compile
g++ skimming.cc -o skimming.exe `root-config --cflags` `root-config --libs`

#inputs
infile=/afs/cern.ch/work/y/yanchu/graviton/CMSSW_8_0_10/src/CMGTools/XZZ2l2nu/cfg/NonResBG_dt/nonresbkg/muoneg.root
outfile=/afs/cern.ch/work/y/yanchu/graviton/CMSSW_8_0_10/src/CMGTools/XZZ2l2nu/cfg/NonResBG_dt/nonresbkg/muonegtrgsf.root

echo -- Command: ./skimming.exe $infile $outfile 
  
./skimming.exe $infile $outfile

