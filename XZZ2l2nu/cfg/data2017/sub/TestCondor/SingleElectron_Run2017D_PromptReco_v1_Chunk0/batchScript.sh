#!/bin/bash

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh
slc6_amd64_gcc530

echo 'environment:'
echo
export X509_USER_PROXY=$CMSSW_BASE/src/myproxy
env | sort
echo 'copying job dir to worker'
cd $CMSSW_BASE/src
eval `scramv1 ru -sh`
cd -
echo 'running'
python $CMSSW_BASE/src/PhysicsTools/HeppyCore/python/framework/looper.py pycfg.py config.pck --options=options.json
echo
echo 'sending root files to remote dir'
export LD_LIBRARY_PATH=/usr/lib64:$LD_LIBRARY_PATH 

eos mkdir /eos/cms/store/user/tocheng/X2l+MET+jets/TestCondor/SingleElectron_Run2017D_PromptReco_v1/SingleElectron_Run2017D_PromptReco_v1_Chunk0

eos cp -r Loop/ /eos/cms/store/user/tocheng/X2l+MET+jets/TestCondor/SingleElectron_Run2017D_PromptReco_v1/SingleElectron_Run2017D_PromptReco_v1_Chunk0

if [ $? -ne 0 ]; then
   echo 'ERROR: problem copying job directory back'
else
   echo 'job directory copy succeeded'
fi

