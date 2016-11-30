#!/bin/sh


tag="Test1_Photon_GJHT_RhoWt_QCD50plus_"
#cutChains="loosecut"
cutChains="tight"
logdir="log_ph_b2h36p1fbinv"

mkdir -p $logdir


for cutChain in $cutChains;
do
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY  --test &> ${logdir}/${tag}${cutChain}_log.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain"  --test &> ${logdir}/${tag}${cutChain}.log &
#  ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY --Blind  --test &> ${logdir}/${tag}${cutChain}.log &
done
