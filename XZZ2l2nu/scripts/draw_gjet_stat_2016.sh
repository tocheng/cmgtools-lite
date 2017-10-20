#!/bin/sh


tag="ReMiniAODCRScaleMoreSig_"

channels="mu el"
#cutChains="SR"
#cutChains="SR CR1"
#cutChains="SRdPhiGT0p5 SR"
cutChains="tight"
logdir="log"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do

      python stack_dtmc_skim_gjets_stat_2016dataVS2016bkg.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY --muoneg &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_dyGJets_plot.log &

      python stack_dtmc_skim_gjets_stat_2016dataVS2016bkg.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_dy_plot.log &

   done
done
