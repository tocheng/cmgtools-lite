#!/bin/sh

#tag="srdphigt0p5scale_"
#tag="ReMiniSummer16_DT_PhReMiniMCRcFixXsec_"
#tag="PAS_ReMiniSummer16_MC_PhReMiniMCRcFixXsec_"
tag="PAS1_ReMiniSummer16_DT_PhReMiniMCRcFixXsec_"
#tag="PAS_ReMiniSummer16_DT_PhReMiniMCRcFixXsec_"
#tag="tunezpt_"
#tag="mtexcess_"
#tag="cuts_"
#tag="debugMtFixZMassFineBinNoPhMETSub_"
#tag="debugMtFixZMassFineBinGraphSmooth_"
#tag="debugMtFixZMassFineBinSmooth_"
#tag="debugMtFixZMassFineBin_"
#tag="debugMtFixZMassGraphSmooth_"
#tag="debugMtFixZMassGraph_"
#tag="debugMtFixV1_"
#tag="debugMtRcNoSmooth_"
#tag="debugMt_"
#tag="debugMtdPhiWt_"
#tag="cutflow_"
#tag="ReMiniSummer16_DT_PhReRecoMCRcFixXsec_"
#tag="ReMiniSummer16_DT_PhReMiniMCRcFixXsec_"
#tag="test2PhNoMCSub_"
#tag="test2hltFixZGNuNu_"
#tag="test2hltFixXsec_"
#tag="test2hltPhMCRcFixXsec_"
#tag="test2hltPhMCRcFixZGNuNu_"
#tag="testReReco_2hltFixZGNuNu_"
#tag="testReReco_2hltFixXsec_"
#tag="testReReco_2hltPhMCRcFixXsec_"
#tag="ReMiniSummer16_DT_"
#tag="ReMiniSummer16_MC_"
#tag="scale_mc_"
#tag="test_ReMiniSummer16_DT_PhReRecoRePreSkim"
#tag="test_ReMiniSummer16_DT_PhAllCor"
#tag="test_ReMiniSummer16_DT_PhAllCorV2"
#tag="test_NoZZNNLOCorr_"
#tag="test_ZZ2l2nu0p5_"
#tag="test_ReMiniSummer16_DT_PhAllCorV2_"
#tag="test_LepVeto_ReMiniSummer16_DT_PhReRecoRePreSkim_"
#tag="test_ReMiniSummer16_DT_PhReRecoRePreSkim_"
#tag="test_ReMiniSummer16_DT_PhReRecoReSkim_"
#tag="test_ReMiniSummer16_DT_PhAllCor_"
#tag="test_ReMiniSummer16_DT_PhNoCorr_"
#tag="test_ReMiniSummer16_DT_PhReReco_"
#tag="test_ReMiniSummer16_DT_"
#tag="study_signal_eff_"
#tag="test_PhNewDY_NewDJ_"
#tag="test_NoWZ2l2q_NewDJ_"
#tag="test_NoWZ3l1nu_NewDJ_"
#tag="test_ZZ2l2nu0p5_NewDJ_"
#tag="test_NoZZ2l2q_NewDJ_"
#tag="test_NoZZ4l_NewDJ_"
#tag="test_NoggZZ_NewDJ_"
#tag="test_NoTTZ_NewDJ_"
#tag="test_NewDJ_"
#tag="test_NewDJNoMETSub_"
#tag="test29_DtRecalib_"
#tag="trigStudyHighLepPt_"
#tag="GJetsSumm16JEC_badMuonFilter_"
#tag="MCSummer16JECNLOZJETHighLepPtCutAllSig_"
#tag="MCSummer16JECHighLepPt_"
#tag="test_"

channels="mu el"
#channels="el"
#channels="mu"
#cutChains="loosecut lepaccept tight tightzpt50 tightzpt100 SR CR1 CR2 CR3"
#cutChains="tight"
#cutChains="tightzpt50"
#cutChains=" SRMtLt180"
#cutChains=" SRmetParaLT0"
#cutChains=" SRmetParaGT0"
#cutChains=" SRmetParaLT50"
#cutChains="SRMtGt1Lt1p05 "
#cutChains="SRMtGt0p8Lt0p95 "
#cutChains=" SRdPhiGT0p3 "
#cutChains=" SRdPhiLT0p5 SRdPhiGT0p5 "
#cutChains=" SRdPhiLT0p5 "
#cutChains=" zpt100dPhiLT0p5 zpt100dPhiGT0p5LT1 zpt100dPhiGT1LT1p5 zpt100dPhiGT1p5LT2 zpt100dPhiGT2 "
#cutChains=" SRdPhiGT0p5LT1 SRdPhiGT1LT1p5 SRdPhiGT1p5LT2 SRdPhiGT2 "
#cutChains=" SRMetGT55 "
#cutChains=" SRmetParaLT100 SRdPhiGT0p5 "
#cutChains="tightzpt100 SR SRMtLt180 SRmetParaLT0 SRmetParaGT0 SRmetParaLT50 SRmetParaLT100 SRdPhiGT0p5 SRdPhiGT0p3"
#cutChains="tightzptgt100lt400"
#cutChains="tightzptgt50lt200"
#cutChains="tightzptgt55metgt125"
#cutChains="SRmetParaLTm80"
cutChains="SRdPhiGT0p5"
#cutChains="SR"
#cutChains="CR"
#cutChains="SR tightzpt50"
#cutChains="SR tightzpt50 tightzpt100"
#cutChains="CR1 CR2 CR3"
#cutChains="CR1"
#cutChains="CR2 CR3"
#cutChains="tightzpt50 tightzpt100 tightzpt100met50"
#cutChains="tightzpt50 tightzpt100"
#cutChains="tightzpt100 tightzpt100met50"
#cutChains="tightzpt100met50"
#cutChains="tightzpt150 tightzpt100"
#cutChains="tightzpt150 tightzpt200 tightzpt100met50"
#cutChains="tightzpt300"
#cutChains="tightzpt200"
#cutChains="tightzpt100"
#cutChains="tightzpt50"
#cutChains="tight"
#cutChains="tight tightzpt50 tightzpt100"
#cutChains="nonreso_zptgt70"
#cutChains="nonreso_zptgt50"
#cutChains="nonreso_zptgt0 nonreso_zptgt10 nonreso_zptgt20 nonreso_zptgt30 nonreso_zptgt40"
#cutChains="nonreso_zptgt50_metlt20 nonreso_zptgt50_metlt30 nonreso_zptgt50_metlt50 "

#cutChains="nonreso_zptgt50 nonreso_zptgt50_metlt100 tightzpt50"
#cutChains="nonreso_zptgt70 tightzpt50"
#cutChains="nonreso_zptgt50_metlt100 tightzpt50"
#cutChains="tightzptgt50lt200"
logdir="log"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do  
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> ${logdir}/${tag}${cutChain}_mc_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel"  --LogY &> ${logdir}/${tag}${cutChain}_mc_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &

#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind --test &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind --LogY  --test &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --LogY  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &

#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel"  --LogY --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel"  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
#       ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --muoneg  --LogY --test &> ${logdir}/${tag}${cutChain}_emu_log_${channel}.log &
       ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets  --muoneg --LogY --test &> ${logdir}/${tag}${cutChain}_emu_gjet_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets  --muoneg  --test &> ${logdir}/${tag}${cutChain}_emu_gjet_log_${channel}.log &
#       ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets  --LogY --test &> ${logdir}/${tag}${cutChain}_emu_gjet_log_${channel}.log &
#       ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --dyGJets  --muoneg --LogY --test &> ${logdir}/${tag}${cutChain}_emu_gjet_bld_log_${channel}.log &

   done
done
