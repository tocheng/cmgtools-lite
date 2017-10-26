#!/bin/sh

# contact: Hengne.Li@cern.ch, lihengne@gmail.com

# step 1: run combine to calculate limits, for all kinds of signal hypotheses

# Narrow-width datacards:
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_BulkGrav_narrow_mT

# Wide width datacards, gg->G, width=0GeV, 0.1*mG, 0.2*mG, 0.3*mG:
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PB_width0_mT
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PB_width0p1_mT
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PB_width0p2_mT
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PB_width0p3_mT

# Wide width datacards, qq->G, width=0GeV, 0.1*mG, 0.2*mG, 0.3*mG:
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PBqqbar_width0_mT
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PBqqbar_width0p1_mT
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PBqqbar_width0p2_mT
#   ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_Graviton2PBqqbar_width0p3_mT

# loop over all these dirs, run combine:
# Note: this will take you 9 CPU threads and run in parallel.
for dd in ReMiniAODCRScaleMoreSig_SRdPhiGT0p5_perBinStatUnc_*;
do
   cd $dd;
   ./runAsymptotic.sh &> run.log &
   # note above, same runAsymptotic.sh in each dir, the run.log in each dir records running log.
   cd -;
done

# wait the above combines to be finished before going to the next steps. 
wait ;

# comment out all above lines if you have already run combine, want to go to step 2 plotting directly.

# step 2: plot limits

# this will draw the limits plots, save in .pdf/.png/.C/.root files, all store in the current directory.

# plot limits for narrow width 
./plotLimit_BulkGrav_narrow.py -l -b -q --unblind --cutChain SRdPhiGT0p5 --perBinStatUnc

# plot limits for wide width: gg->G
./plotLimit_spin2.py -l -b -q --unblind --cutChain SRdPhiGT0p5 --perBinStatUnc

# plot limits for wide width: qq->G
./plotLimit_spin2_qqbar.py -l -b -q --unblind --cutChain SRdPhiGT0p5 --perBinStatUnc



