

   dir=$1
   mH=$2
   cut=$3
  
   cd $1

   combineCards.py ee_SR=mX${mH}ZZ2l2nu_ee_${cut}.txt mm_SR=mX${mH}ZZ2l2nu_mm_${cut}.txt > mX${mH}ZZ2l2nu_${cut}.txt

#   combine -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_SR.txt --run blind --rMax 10 --noFitAsimov
#   combine -n ee_SR -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_ee_SR.txt --run blind --rMax 10 --noFitAsimov
#   combine -n mm_SR -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_mm_SR.txt --run blind --rMax 10 --noFitAsimov

   rmax=0.0001
   rabsacc=0.00001

   
   cmd="combine -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_${cut}.txt --rMax $rmax --rAbsAcc $rabsacc --noFitAsimov"
   echo "command: $cmd"
   ${cmd}

   cmd="combine -n ee_${cut} -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_ee_${cut}.txt --rMax $rmax --rAbsAcc $rabsacc --noFitAsimov"
   echo "command: $cmd"
   ${cmd}
   
   cmd="combine -n mm_${cut} -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_mm_${cut}.txt --rMax $rmax --rAbsAcc $rabsacc --noFitAsimov"
   echo "command: $cmd"
   ${cmd}

   cd -
