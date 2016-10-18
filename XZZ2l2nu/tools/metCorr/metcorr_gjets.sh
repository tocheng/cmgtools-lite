#!/bin/sh

#inputs
inputdir=/home/heli/XZZ/80X_20161006_light
outputdir=/home/heli/XZZ/80X_20161006_light_Skim
#inputdir=/home/heli/XZZ/80X_20160927_light
#outputdir=/home/heli/XZZ/80X_20160927_light_Skim
config=config/parameters_light_gjets

mkdir -p ${outputdir}

gmake all

njob="0"

#for infile in $inputdir/GJet_Pt_20toInf_DoubleEMEnriched/vvTreeProducer/tree.root ; 
#for infile in $inputdir/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root ; 
for infile in $inputdir/SinglePhoton_Run2016B2G_PromptReco/vvTreeProducer/tree.root ; 
do
  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\//}"

  # options for outputs
  #outfile="${outfile/\/vvTreeProducer\/tree/}"
  outfile="${outfile/\/vvTreeProducer\/tree/_RcDataB2GNewFilterLepVeto}"
  #outfile="${outfile/\/vvTreeProducer\/tree/_newFilterLepVetoNoRecoil}"
  #outfile="${outfile/\/vvTreeProducer\/tree/_newFilterLepVetoNoRecoil}"
  #outfile="${outfile/\/vvTreeProducer\/tree/_test}"

  inSkimFile=${infile/vvTreeProducer\/tree.root/skimAnalyzerCount\/SkimReport.txt}

  #echo $inSkimFile
  AllEvents=`grep "All Events" ${inSkimFile} | awk {'print $3'}`
  SumWeights=`grep "Sum Weights" ${inSkimFile} | awk {'print $3'}`

  if [ -z $AllEvents ]; then
    echo Fail to get All Events from file ${inSkimFile}
    continue
  fi
  if [ -z $SumWeights ]; then
    SumWeights=$AllEvents
  fi

  echo -- Input file: $infile
  echo -- Output file: $outfile
  echo -- AllEvents: $AllEvents , SumWeights: $SumWeights
  echo -- Selection: $selection
  echo -- Command: ./bin/metcorr.exe $config $infile $outfile $AllEvents $SumWeights

  ./bin/metcorr.exe $config $infile $outfile $AllEvents $SumWeights &> ${outfile}.skim.log &

  njob=$(( njob + 1 ))
  if [ "$njob" -eq "100" ]; then
   # wait
    njob="0"
  fi

done

