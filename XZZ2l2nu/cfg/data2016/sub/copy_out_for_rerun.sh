#!/bin/sh


file=runs_in_rereco_file_singleelectron.txt
instr=/data2/XZZ2/80X_20161029_Chunks/dt_emu_rereco/SingleElectron_Run2016B_23Sep2016_v2_Chunk
outdir=/data2/XZZ2/80X_20161029_Chunks/dt_emu_rereco_resub

#file=runs_in_rereco_file_singlemuon.txt
#instr=/data2/XZZ2/80X_20161029_Chunks/dt_emu_rereco/SingleMuon_Run2016B_23Sep2016_v2_Chunk
#outdir=/data2/XZZ2/80X_20161029_Chunks/dt_emu_rereco_resub

#file=runs_in_rereco_file_singlephoton.txt
#instr=/data2/XZZ2/80X_20161029_Chunks/gjetsdt/SinglePhoton_Run2016B_23Sep2016_v2_Chunk
#outdir=/data2/XZZ2/80X_20161029_Chunks/gjetsdt_resub

run=274157

mkdir -p $outdir

for ff in `grep $run $file -B 1 | grep store `; 
do 
  dd=`grep -l $ff ${instr}*/config.pck | sed "s/\/config.pck//g"` ;
  echo $dd; 

  mv $dd $outdir/

done 
