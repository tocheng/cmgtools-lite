#!/bin/sh

dds="dt3phalc? "

#for nn in `seq 1 1 1000`;
#do 
  for dd in $dds;
  do
    ./check.sh $dd &> ck_${dd}.log &
  done
  #sleep 1800
#  wait
#done

