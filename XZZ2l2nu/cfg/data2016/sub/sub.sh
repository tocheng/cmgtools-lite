#!/bin/sh

#heppy_batch.py -o dt1 run_xzz2l2nu_80x_cfg_loose_dt.py -b 'bsub -q ssssssss  < ./batchScript.sh'
#heppy_batch.py -o dt5 run_xzz2l2nu_80x_cfg_loose_dt.py -b 'bsub -q ssssssss  < ./batchScript.sh'
#heppy_batch.py -o dt3 run_xzz2l2nu_80x_cfg_loose_dt.py -b 'bsub -q ssssssss  < ./batchScript.sh'
#heppy_batch.py -o dt2 run_xzz2l2nu_80x_cfg_loose_dt.py -b 'bsub -q ssssssss  < ./batchScript.sh'
#heppy_batch.py -o dt3 run_xzz2l2nu_80x_cfg_loose_dt.py -b 'bsub -q ssssssss  < ./batchScript.sh'
heppy_batch.py -o dt8 run_xzz2l2nu_80x_cfg_photon_dt.py -b 'bsub -q ssssssss  < ./batchScript.sh'
#heppy_batch.py -o dt6 run_xzz2l2nu_80x_cfg_photon_dt_trigOnly.py -b 'bsub -q ssssssss  < ./batchScript.sh'

