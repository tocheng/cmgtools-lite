#!/bin/sh


tag="GJets_"

channels="el mu"
cutChains="tightzpt50 SRdPhiGT0p5"

logdir="log_gjets_35p9"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do

     python stack_dtmc_skim_gjets_stat_v2.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --muoneg --dyGJets &> ${logdir}/${tag}${cutChain}_log_${channel}_blind_plot.log &

   done
done
