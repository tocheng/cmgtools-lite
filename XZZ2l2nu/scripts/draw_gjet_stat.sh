#!/bin/sh


tag="ReMiniAODCRScaleMoreSig_"
#tag="ReMiniAODCRScale_"
#tag="ReMiniAODNoTune_"
#tag="ReMiniAODNoMETCutVary_"

channels="mu el"
#cutChains="SR"
#cutChains="SR CR1"
#cutChains="SRdPhiGT0p5 SR"
cutChains="SRdPhiGT0p5 "
#cutChains="SRzptGT200"
logdir="log"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do

     python stack_dtmc_skim_gjets_stat_v2.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY --muoneg --doSys &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_sys_plot.log &


   done
done
