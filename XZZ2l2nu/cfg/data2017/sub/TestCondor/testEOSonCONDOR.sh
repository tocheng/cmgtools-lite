#!/bin/bash

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

env | sort

echo `pwd`

echo "eos mkdir /eos/cms/store/user/tocheng/TestCondor"
eos mkdir /eos/cms/store/user/tocheng/TestCondor

echo "eos cp `pwd`/inputFile1.txt /eos/cms/store/user/tocheng/TestCondor/"
eos cp `pwd`/$1 /eos/cms/store/user/tocheng/TestCondor/

cp inputFile3.txt inputFile2.txt

eos cp -r `pwd`/Dir/ /eos/cms/store/user/tocheng/TestCondor/

remsize=$(eos find --size /eos/cms/store/user/tocheng/TestCondor/inputFile1.txt | cut -d= -f3)
locsize=$(cat `pwd`/inputFile1.txt | wc -c)
ok=$(($remsize==$locsize))
if [ $ok -ne 1 ]; then
      echo "Problem with copy (file sizes don't match), will retry in 30s"
fi

xrdcp root://eoscms.cern.ch//eos/cms/store/user/tocheng/Cert_294927-302663_13TeV_PromptReco_Collisions17_JSON.txt Cert_294927-302663_13TeV_PromptReco_Collisions17_JSON.txt
