#!/bin/sh


for dd in `ls | grep dt2| grep -v run | grep -v log`; 
do 
  echo "$dd: "`ls -l $dd | grep Chunk| grep -v " 2 heli" |wc -l` ; 
done

