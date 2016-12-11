#!/bin/sh


#tag="Ph_ResBosRefit_Rc36p46_NoISR_"
#tag="Ph_ResBosRefit_Rc36p46_"
tag="Test_Ph_ResBosRefit_Rc36p46_"
#cutChains="loosecut"
cutChains="tight"
logdir="log_ph_36p46"

mkdir -p $logdir


for cutChain in $cutChains;
do
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --LogY  &> ${logdir}/${tag}${cutChain}_SepProc_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess &> ${logdir}/${tag}${cutChain}_SepProc.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY  --test &> ${logdir}/${tag}${cutChain}_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain"  --test &> ${logdir}/${tag}${cutChain}.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --LogY  --test &> ${logdir}/${tag}${cutChain}_SepProc_log.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --test &> ${logdir}/${tag}${cutChain}_SepProc.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="mu" --LogY  --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_mu_log.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="mu" --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_mu.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="el" --LogY  --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_el_log.log &
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="el" --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_el.log &
done
