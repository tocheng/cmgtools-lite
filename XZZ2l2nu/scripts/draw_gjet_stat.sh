#!/bin/sh


tag="GJets_"

channels="mu el"
cutChains="tightzpt50"
#"SRdPhiGT0p5"
#tightzpt50 "

logdir="log_gjets_14p1"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do

     #python stack_dtmc_skim_gjets_stat_2017dataVS2016bkg.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --muoneg --dyGJets --Blind &> ${logdir}/${tag}${cutChain}_log_${channel}_blind_plot_dyGJets.log &

     python stack_dtmc_skim_gjets_stat_2017dataVS2016bkg.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --muoneg --Blind  &> ${logdir}/${tag}${cutChain}_log_${channel}_blind_plot.log &

   done
done
