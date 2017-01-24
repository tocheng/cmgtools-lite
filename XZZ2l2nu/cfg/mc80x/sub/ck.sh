#!/bin/sh

#dds="mc_zjet mc_zjetext mc_z mc_w mc_t"
dds=" mc_ph_gjet mc_ph_physmet mc_ph_qcd "

for nn in `seq 1 1 100`;
do 
  for dd in $dds;
  do
    ./check.sh $dd &> ck_${dd}.log &
  done
  #sleep 1800
  wait
done

