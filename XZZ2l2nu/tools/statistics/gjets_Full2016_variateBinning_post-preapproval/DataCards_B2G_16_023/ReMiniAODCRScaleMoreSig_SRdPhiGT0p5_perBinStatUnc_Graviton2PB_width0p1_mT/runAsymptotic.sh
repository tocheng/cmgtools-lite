#!/bin/sh

# the cut string in the datacards file names
cut=SRdPhiGT0p5
  
# loop over all mass points run combine
for mH in 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1800 2000 2500;
do 
   #combine ee and mm datacards
   combineCards.py ee_SR=mX${mH}ZZ2l2nu_ee_${cut}.txt mm_SR=mX${mH}ZZ2l2nu_mm_${cut}.txt > mX${mH}ZZ2l2nu_${cut}.txt

   # some conditional modifications of options to make the calculation converge
   rmax=0.0001
   rabsacc=0.00001
   if [ "$mH" -lt "600" ]; then 
     rmax=0.01
     rabsacc=0.0005
   fi
   
   # run combine for ee+mm
   cmd="combine -n _eemm_${cut} -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_${cut}.txt --rMax $rmax --rAbsAcc $rabsacc --noFitAsimov"
   echo "command: $cmd"
   ${cmd}

   # run combine for ee
   cmd="combine -n _ee_${cut} -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_ee_${cut}.txt --rMax $rmax --rAbsAcc $rabsacc --noFitAsimov"
   echo "command: $cmd"
   ${cmd}
   
   # run combine for mm
   cmd="combine -n _mm_${cut} -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_mm_${cut}.txt --rMax $rmax --rAbsAcc $rabsacc --noFitAsimov"
   echo "command: $cmd"
   ${cmd}

done



