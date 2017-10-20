#!/bin/sh


tag="GJets2017_"

channels="mu el"
cutChains="tight"
#"SRdPhiGT0p5"
#tightzpt50 "

logdir="log"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do

     python stack_dtmc_skim_gjets_stat_2017dataVS2016bkg.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --Blind &> ${logdir}/${tag}${cutChain}_blind_log_${channel}_dy_plot.log &

     python stack_dtmc_skim_gjets_stat_2017dataVS2016bkg.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --muoneg --Blind &> ${logdir}/${tag}${cutChain}_blind_log_${channel}_dyGjets_plot.log &

   done
done

