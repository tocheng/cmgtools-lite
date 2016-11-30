#!/bin/sh


#tag="Test_36p22_"
#tag="36p22_"
#tag="Test_36p22_DYRes_"
tag="36p22_DYRes_"
#channels="mu"
channels="all mu el"
#cutChains="tight tightzpt100 tightzpt100met50"
#cutChains="tightzpt100 tightzpt100met50"
cutChains="tight"
#cutChains="tightzptlt200"
logdir="log_36p22"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      ./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
      ./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" &> ${logdir}/${tag}${cutChain}_${channel}.log &
      #./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY  --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --Blind  --test &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &

   done
done
