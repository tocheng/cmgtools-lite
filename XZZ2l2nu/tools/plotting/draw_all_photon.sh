#!/bin/sh


tag="Test_Photon_PhyMET_Rc36p22_"
#tag="Test_Photon_PhyMET_NoZWt_DtAsSig_"
#tag="Test_Photon_PhyMET_ZWt_"
#tag="Test_Photon_"
#tag="Test_Photon_GJetsEM_NoQCD_"
#cutChains="loosecut"
cutChains="tight"
logdir="log_ph_36p22"

mkdir -p $logdir


for cutChain in $cutChains;
do
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY  --test &> ${logdir}/${tag}${cutChain}_log.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain"  --test &> ${logdir}/${tag}${cutChain}.log &
#  ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY --Blind  --test &> ${logdir}/${tag}${cutChain}.log &
done
