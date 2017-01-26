#!/bin/sh

#dds="dt2_el dt2_mu dt_ph_b dt_ph_c dt_ph_d dt_ph_e dt_ph_f dt_ph_g dt_ph_h dt_b2h_el  dt_b2h_el_d dt_b2h_el_e dt_b2h_el_f dt_b2h_el_g dt_b2h_el_h  dt_b2h_mu  dt_b2h_mu_e dt_b2h_mu_f dt_b2h_mu_g dt_b2h_mu_h"
dds="dt2_el dt2_mu"
for nn in `seq 1 1 100`;
do 
  for dd in $dds;
  do
    ./check.sh $dd &> ck_${dd}.log &
  done
  #sleep 1800
  wait
done

