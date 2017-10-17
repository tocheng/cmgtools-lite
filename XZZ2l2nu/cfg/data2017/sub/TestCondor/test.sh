#!/bin/sh

if [ $(echo $(condor_submit test.sub) | grep "fail" ; echo $?) -eq 1 ]; then
   echo 'job failed'
   sleep 5
   echo $(condor_submit test.sub)
else
  echo 'job succeed'
fi
 
