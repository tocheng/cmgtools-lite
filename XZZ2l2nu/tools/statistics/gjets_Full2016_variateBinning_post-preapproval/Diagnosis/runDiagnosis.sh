
indir=$1
mX=$2

cp $1/mX${mX}* .

combineCards.py ee_SR=mX${mX}ZZ2l2nu_ee_SR.txt mm_SR=mX${mX}ZZ2l2nu_mm_SR.txt > mX${mX}ZZ2l2nu_SR.txt

combine -M MaxLikelihoodFit --saveNormalizations --saveShapes --saveWithUncertainties -m ${mX} mX${mX}ZZ2l2nu_SR.txt -t -1 --expectSignal 0
mv mlfit.root mlfit_b-only_met.root

#combine -M MaxLikelihoodFit --saveNormalizations --saveShapes --saveWithUncertainties -m ${mX} mX${mX}ZZ2l2nu_SR.txt -t -1 --expectSignal 0
#mv mlfit.root mlfit_b-only.root
#combine -M MaxLikelihoodFit --saveNormalizations --saveShapes --saveWithUncertainties -m ${mX} mX${mX}ZZ2l2nu_SR.txt -t -1 --expectSignal 0.5
#mv mlfit.root mlfit_s+b.root


#combine -M MaxLikelihoodFit --saveNormalizations --saveShapes --saveWithUncertainties -m ${mX} mX${mX}ZZ2l2nu_SR.txt 
#mv mlfit.root mlfit_obs.root

