

for mH in 600 800 1000 1200 1400 1600 1800 2000
  

#for mH in 1600

   do  

   cd $1
  
   echo ${mH}
   echo 'ee+mumu vv '
 
   combineCards.py mX${mH}ZZ2l2nu_ee_${2}_sr.txt mX${mH}ZZ2l2nu_mm_${2}_sr.txt > mX${mH}ZZ2l2nu_${2}_sr.txt

   combine -n $2 -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_${2}_sr.txt --run blind --rMax 4 --noFitAsimov

   cd -

   echo '   '

   done


