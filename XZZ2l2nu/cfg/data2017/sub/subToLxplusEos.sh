#!/bin/sh

outdir=$1
config=$2
otherfiles=" pogRecipesData92X.py "

heppy_batch.py -o ${outdir} ${config} -r /store/user/tocheng/X2l+MET+jets/${outdir} -b 'bsub -q sssss < ./batchScript.sh' -n
for dd in ${outdir}/* ; 
do cp $otherfiles ${dd}/;
done

cd $outdir
heppy_check.py * -b "bsub -q 1nd"
cd ../
