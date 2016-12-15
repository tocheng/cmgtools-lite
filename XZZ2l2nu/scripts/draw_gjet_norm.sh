#!/bin/sh


tag="GJets_BkgSub_Rc36p46wHLT_"

channels="mu el"
cutChains="tight"
logdir="log_gjets_36p46"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do

#      python stack_dtmc_skim_gjets_dt_plot.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY --muoneg &> ${logdir}/${tag}${cutChain}_log_${channel}_plot.log &

      python stack_dtmc_skim_gjets_dt_plot.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --muoneg &> ${logdir}/${tag}${cutChain}_log_${channel}_plot.log &
 
  done
done
