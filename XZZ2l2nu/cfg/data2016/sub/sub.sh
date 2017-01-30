#!/bin/sh


outdir=dt2_mu
#outdir=dt_ph_b
#outdir=dt_b2h_mu_g
config=run_xzz2l2nu_80x_cfg_loose_dt.py
#config=run_xzz2l2nu_80x_cfg_photon_dt.py
otherfiles=pogRecipes.py

heppy_batch.py -o ${outdir} ${config}  -b 'bsub -q sssss < ./batchScript.sh' -n
for dd in ${outdir}/* ;
do cp $otherfiles ${dd}/;
done

cd $outdir
heppy_check.py * -b "bsub -q 8nh"

cd ../
