#!/bin/sh


for dd in `ls | grep dt3| grep -v run | grep -v log`; 
do 
  echo "$dd: "`ls -l $dd/*Chunk*/vvTreeProducer/tree.root  |wc -l` ; 
done

