#!/bin/sh

run=274157

#dataset=/SingleMuon/Run2016B-23Sep2016-v3/MINIAOD
#dataset=/SingleElectron/Run2016B-23Sep2016-v3/MINIAOD
dataset=/SinglePhoton/Run2016B-23Sep2016-v3/MINIAOD


files=`das_client --limit=0 --query="file dataset=$dataset"`

for ff in $files;
do
   echo $ff
   das_client --limit=0 --query="run file=$ff"
done
