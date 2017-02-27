#!/bin/sh

dds="dt3??alc* "
#dds="dt3mualc? "
#dds="dt3mualc? dt3elalcb"
#dds="dt3mualc[bcefg] dt3elalcb1 "

#nt="0"
#for nn in `seq 1 1 100000`; do 
  for dd in $dds; do
    ./check.sh $dd &> ck_${dd}.log &
  done
#  wait
#  nt=$(( nt + 1 ))
#  echo "n times : ${nt}"
#  sleep 300 
#done

